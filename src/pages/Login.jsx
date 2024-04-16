'use strict';
import { createSignal } from "solid-js";

import { login } from '../helpers/caller'
import { sidebar, navigate} from '../helpers/state'
import { parsePersonalInfo } from '../helpers/parser'
import './Login.css';


export default function(){
  const [bg, setBg] = createSignal("red");
  const username = <input type="text" autocomplete="enrollment-number" placeholder="Enrollment No." id="username" />;
  const password = <input autocomplete="webkiosk-password" type="password" placeholder="Password" id="password" />;
  function handleclick(e){
    e.preventDefault();
    login(username.value, password.value, false)
      .then(cookie=>{
        globalThis.__cookie = cookie;
        parsePersonalInfo(cookie).then(info=>{
          console.log('lgn',info);
          globalThis.__info = {};
          globalThis.__info.PersonalInfo = info;
          sidebar(true);
          navigate('personal-info');
        });
      });
  }
  return (
    <div id="login-form" class="flex justify-center select-none place-items-center h-full w-full overflow-scroll">
      <div class="shape mb-32 mr-16" style='background:linear-gradient(to right,#ff512f,#f09819);'/>
      <div class="shape mt-32 ml-16" style='background:linear-gradient(#1845ad,#23a2f6);'/>
      <form class="z-50 overflow-clip rounded-3xl shadow-black shadow-2xl border-none">
        <h3>Login Here</h3>
        <label for="username">Username</label>
        {username}
        <label for="password">Password</label>
        {password}
        <button class="bg-white px-4 py-2" onclick={handleclick}>Log In</button>
      </form>
    </div>
  );
}

