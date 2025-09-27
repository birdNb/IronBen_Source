<?php

namespace App\Http\Controllers;

use App\Models\User;
use Illuminate\Http\Request;

class TestController extends Controller
{
    //跳转到主页面
    public function test() {
        return response()->view(view:'register');
    }

    //注册选项
    public function creat() {
        return response()->view(view:'userCreat');
    }

    //登录选项
    public function login(Request $request)
    {
        $number = $request->get('number');
        $password = $request->get('password');
        if (User::where('number', $number)->where('password', $password)->exists()) {
            return response()->view(view:'function');
        } else {
            return response("登录失败");
            // return response()->view(view:'register');
        }
    }

    //新建用户并储存用户信息
    public function addUser(Request $request)
    {
        $name = $request->get('name');
        $password = $request->get('password');
        $number = $request->get('number');
        $user = new User();
        $user->number = $number;
        $user->name = $name;
        $user->password = $password;
        $user->save();
        return response()->view(view:'register');
    }
}




