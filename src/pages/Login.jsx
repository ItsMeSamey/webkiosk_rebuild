'use strict';
import { createSignal } from "solid-js";

import { login } from '../helpers/caller'
import { sidebar, navigate} from '../helpers/state'
import { parsePersonalInfo } from '../helpers/parser'
import './Login.css';


export default function(){
  // <link rel="preconnect" href="https://fonts.gstatic.com">
  // <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.4/css/all.min.css">
  // <link href="https://fonts.googleapis.com/css2?family=Poppins:wght@300;500;600&display=swap" rel="stylesheet">
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
        })
      });
  }
  return (
    <>
      <div class="background">
        <div class="shape"></div>
        <div class="shape"></div>
      </div>
      <form id="login-form" class="select-none h-auto w-auto backdrop-blur-3xl rounded-2xl shadow-black shadow-2xl px-16 py-20 border-none" >
        <h3>Login Here</h3>
        <label for="username">Username</label>
        {username}
        <label for="password">Password</label>
        {password}
        <button class="bg-white px-4 py-2" onclick={handleclick}>Log In</button>
      </form>
    </>
  );
}

