#pragma comment (lib,"lua5.1.lib")


#include "lua.h"

#include "lualib.h"

#include "lauxlib.h"

#include <windows.h>

//执行lua脚本
void runhelloTest(const char* lua){
	lua_State* lua_state = luaL_newstate();
	luaL_openlibs(lua_state);
	luaL_dofile(lua_state,lua);
	lua_close(lua_state);
}
static int average_C(lua_State *L)
{
    /* get number of arguments */
   	/* 从lua脚本中获得参数的个数*/
    int n = lua_gettop(L);
    
    double sum = 0;
    int i;
 
    /* loop through each argument */
    for (i = 1; i <= n; i++)
    {
        if (!lua_isnumber(L, i))
        {
            lua_pushstring(L, "Incorrect argument to 'average'");
            lua_error(L);
        }
 
        /* total the arguments */
        sum += lua_tonumber(L, i);
    }
 
    /* push the average */
    lua_pushnumber(L, sum / n); //第一个返回值(平均值)
 
    /* push the sum */
    lua_pushnumber(L, sum); //第二个返回值(总和)
 	
    /* return the number of results */
    return 2;
}

//lua调用C
void LuaCallC()
{
    /* initialize Lua */
    lua_State * L = lua_open();
 
    /* load Lua base libraries */
    luaL_openlibs(L);
 
    /* register our function */
    lua_register(L, "average", average_C);
 
    /* run the script */
    luaL_dofile(L, "average.lua");
 
    /* cleanup Lua */
    lua_close(L);
     
}
void main()
{
	//runhelloTest("hellolua.lua");
	LuaCallC();
	
	getchar();
}