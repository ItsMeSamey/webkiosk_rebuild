'use strict';


async function fetchHtml(response){
  const decoder = new TextDecoder();
  const parser = new DOMParser();
  return parser.parseFromString(
    decoder.decode(
      (await (await (await response).body.getReader()).read()).value
    ),
    "text/html"
  ); 
}

async function login(roll_number, password, is_parent){
  const req = new Request("https://webkiosk.thapar.edu/CommonFiles/UserAction.jsp", {
  "headers": {
    "accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8",
    "accept-language": "en-US,en;q=0.9",
    "cache-control": "no-cache",
    "content-type": "application/x-www-form-urlencoded",
    "pragma": "no-cache",
    "sec-ch-ua": "\"Brave\";v=\"123\", \"Not:A-Brand\";v=\"8\", \"Chromium\";v=\"123\"",
    "sec-ch-ua-mobile": "?0",
    "sec-ch-ua-platform": "\"Linux\"",
    "sec-fetch-dest": "document",
    "sec-fetch-mode": "navigate",
    "sec-fetch-site": "same-origin",
    "sec-fetch-user": "?1",
    "sec-gpc": "1",
    "upgrade-insecure-requests": "1"
  /*  */ },
  "referrer": "https://webkiosk.thapar.edu/index.jsp",
  "referrerPolicy": "strict-origin-when-cross-origin",
  "body": "txtuType=Member+Type&UserType=S&txtCode=Enrollment+No&MemberCode=102303535&txtPin=Password%2FPin&Password=1234567890&BTNSubmit=Submit",
  "method": "POST",
  "mode": "cors",
});
  const response = fetch("https://webkiosk.thapar.edu/CommonFiles/UserAction.jsp", {
    "headers": {
      "accept": "*/*",
      "content-type": "application/x-www-form-urlencoded",
    },
    "body": "UserType="+(is_parent?"P":"S")+"&MemberCode="+roll_number+"&Password="+password,
    "method": "POST",
  "credentials": "include",
  withCredentials: true
  });
  console.log(req);
  console.log((await response).headers.getSetCookie());
  console.log(await response);
  for (const pair of (await response).headers.entries()) {
    console.log(`${pair[0]}: ${pair[1]}`);
  }
  return await fetchHtml(response);
}

login('102303535', '1234567890').then(console.log);

