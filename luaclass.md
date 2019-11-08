```
--类的声明，这里声明了类名还有属性，并且给出了属性的初始值
Class = {x=0,y=0}
--设置元表的索引，想模拟类的话，这步操作很关键
Class.__index = Class
--构造方法，构造方法的名字是随便起的，习惯性命名为new()
function Class:new(x,y)
     local self = {}  --初始化self，如果没有这句，那么类所建立的对象如果有一个改变，其他对象都会改变
     setmetatable(self, Class)  --将self的元表设定为Class
     self.x = x   --属性值初始化
     self.y = y
     return self  --返回自身
end

--这里定义类的其他方法
function Class:test()
 print(self.x,self.y)
end

function Class:plus()
 self.x = self.x + 1
 self.y = self.y + 1
end

--require 'Class'

--声明了新的属性Z  
SubClass = {z = 0}  
--设置元表为Class  
setmetatable(SubClass, Class)  
--还是和类定义一样，表索引设定为自身  
SubClass.__index = SubClass  
--这里是构造方法
function SubClass:new(x,y,z)  
   local self = {}             --初始化对象自身  
   self = Class:new(x,y)       --将对象自身设定为父类，这个语句相当于其他语言的super ，可以理解为调用父类的构造函数 
   setmetatable(self, SubClass)    --将对象自身元表设定为SubClass类  
   self.z= z                   --新的属性初始化，如果没有将会按照声明=0  
   return self  
end  

--定义一个新的方法  
function SubClass:go()  
   self.x = self.x + 10  
end  

--重定义父类的方法，相当于override
function SubClass:test()  
     print(self.x,self.y,self.z)  
end



local a = Class:new() -- 首先实例化父类的对象，并调用父类中的方法
a:plus()
a:test()

a = SubClass:new()    -- 然后实例化子类对象
a:plus()            -- 子类对象可以访问到父类中的成员和方法
a:go()                -- 子类对象调用子类中的新增方法
a:test()            -- 子类对象调用重写的方法
```