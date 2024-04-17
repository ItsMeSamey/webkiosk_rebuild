'use strict';

async function login(roll_number, password, is_parent){
  const response = await fetch("/auth/" +roll_number+"/"+password+"/"+(is_parent ? "P": "S"));
  window.localStorage.setItem('auth', JSON.stringify({e:roll_number,p:password,t:is_parent}));
  if (response.ok) {
    const decoder = new TextDecoder();
    const cookie = decoder.decode((await (await (response.body.getReader())).read()).value);
    window.localStorage.setItem('cookie', JSON.stringify({t:Date.now(),v:cookie}));
    globalThis.__.cookie = cookie;
    return cookie;
  }
}

async function call(cookie, path){
  const response = await fetch("/api/" + cookie + "/" + path);
  if (response.ok) {
    const decoder = new TextDecoder();
    const parser = new DOMParser();
    const dom = parser.parseFromString(decoder.decode((await (await (response.body.getReader())).read()).value), "text/html");
    return dom;
  }
}

async function getcookie(){
  const cookie = JSON.parse(window.localStorage.getItem('cookie'));
  if (Date.now - cookie.t <10240000){
    return cookie.v;
  } else {
    const logindata = window.localStorage.getItem('auth', );
    return login();
  }
}

export { login, call, getcookie };

