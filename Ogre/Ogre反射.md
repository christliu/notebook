# OGRE反射

Ogre实现了简单的反射。

## OGRE反射模型

Ogre当中用到的反射模型比较简单，利用c++多态的特性，将反射的转换通过虚函数到子类型当中实现。

Ogre反射的目的：

~~~c++
enum FontType
{
    FT_TRUETYPE=1,
    FT_IMAGE=2
};
class Font:
{
    public:
    	/*
    	String getParameter(const String& name) const;
    	bool setParameter(const String& name, const String& value);
    	*/
    private:
    	FontType mType; 
}
~~~

Font类当中有一个私有成员。希望做到的是能够通过Font.getParameter(”Type“) 和 Font.setParameter("Type", "FT_TRUETYPE")这种调用方式直接get或者set成员变量。

## OGRE反射模型的实现

Ogre通过一个基类：StringInterface来实现反射。上面的Font只需要继承StringInterface，并且声明mType这种类型的get和set方法即可以实现反射的目的。

### StringInterface实现

反射模型当中提到的getParamter和setParamter这两个方法就直接定义并且实现在了StringInterface基类当中。我们先看一下这两个方法的实现：

~~~c++
class StringInterface
{
    String getParameter(const String& name) const
    {
        const ParamDictionary* dict = getParamDictionary();
        if(dict)
        {
            const ParamCommand* cmd = dict->getParamCommand(name);
            if(cmd) return cmd->doGet(this);
        }
        return "";
    }
    
    bool StringInterface::setParameter(const String& name, const String& value)
    {
        ParamDictionary* dict = getParamDictionary();
        if(dict)
        {
            ParamCommand* cmd = dict->getParamCommand(name);
            if(cmd)
            {
                cmd->doSet(this, value);
                return true;
            }
        }
        return false;
    }
}
~~~

代码很简单，我们可以看到，其实StringInterface把对某个成员变量的get和set都印象到了一个叫做ParamCommand的对象上面，通过doGet和doSet来实现真正的get和set操作。而如果这个印射失败的话，会直接返回默认值。

我们有理由猜测，Font内部会对每个成员变量的类型，定义一个继承ParamCommand的新类型，并且在新类型当中实现doGet和doSet方法。然后将这些新定义的类都通过ParamDictionary存储在类空间当中。思维很清晰了，我们来看看Font的初始化的实现。

~~~cpp
class Font
{
    class CmdType: public ParamCommand
    {
        public:
        	String doGet(const void* target) const;
        	void doSet(void* target, const String&val);
    }
    Font()
    {
    	if(createParamDictionary("Font"))
    	{
        	ParamDictionary* dict = getParamDictionary();
        	dict->addParameter(
        		ParameterDef("type", "'truetype' or 'image' based font", 				PT_STRING), &msTypeCmd);
   		}
    }
}
~~~

果然在初始化的时候会构造ParamDictionary，并且往里面塞进去“type"这个name与msTypeCmd的印象，msTypeCmd的定义在类当中，重写了doGet和doSet两个方法。

## OGRE反射模型分析

Ogre的反射实现清晰，代码一目了然，读起来也不费劲。问题在于，每个Font对象在创建的时候都会有初始化反射dict的过程，在Runtime阶段的效率比较低。

相比于现代图形引擎（Unreal、Messiah）等在编译期间就把反射关系建立好的模式来说，Ogre的反射简直就是实验室的玩具。