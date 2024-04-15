'use strict';

async function login(roll_number, password, is_parent){
  const response = await fetch("/auth/" +roll_number+"/"+password+"/"+(is_parent ? "P": "S"));
  if (response.ok) {
    const decoder = new TextDecoder();
    const cookie = decoder.decode((await (await (response.body.getReader())).read()).value)
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

export { login, call };

