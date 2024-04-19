'use strict';
import { createSignal } from 'solid-js';

import { login } from '../helpers/caller'
import { sidebar, navigate} from '../helpers/state'
import { parsePersonalInfo } from '../helpers/parser'
import './Login.css';


export default async function(){
  if (globalThis.__.Login) return globalThis.__.Login;
  const [bg, setBg] = createSignal(true);
  const username = <input type='text' autocomplete='enrollment-number' placeholder='Enrollment No.' id='username' />;
  const password = <input autocomplete='webkiosk-password' type='password' placeholder='Password' id='password' />;
  function handleclick(e){
    try{// console.log('tryinn');
      sidebar(false);
      // console.log('done');
    }catch(error){console.log(error);}
    setBg(false);
    e.preventDefault();
    login(username.value, password.value, false)
      .then(cookie=>{
        globalThis.__cookie = cookie;
        parsePersonalInfo(cookie).then(info=>{
          // console.log('lgn',info);
          globalThis.__.PersonalInfo = info;
          sidebar(true);
          navigate('personal-info');
        });
      });
  }
  globalThis.__.Login = (
    <div class='flex justify-center select-none place-items-center h-full w-full overflow-scroll'>
      <div id='login-form' class='relative z-50 rounded-3xl shadow-black shadow-2xl border-none overflow-clip'>
        <Show when={bg()}>
          <div class='top-4 left-4' style='background:linear-gradient(to right,#ff512f,#f09819);'/>
          <div class='bottom-4 right-4' style='background:linear-gradient(#1845ad,#23a2f6);'/>
        </Show>
        <form class='rounded-3xl w-full h-full'>
          <h3>Login Here</h3>
          <label for='username'>Username</label>
          {username}
          <label for='password'>Password</label>
          {password}
          <button class='bg-white px-4 py-2 mt-6 w-full rounded-full text-black cursor-pointer' onclick={handleclick}>Log In</button>
        </form>
      </div>
    </div>
  );
  return globalThis.__.Login;
}

