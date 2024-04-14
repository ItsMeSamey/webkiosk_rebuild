'use strict';
import { createSignal } from "solid-js";
import { Motion } from 'solid-motionone';

import Login from '../pages/Login';
import Home from '../pages/Home';
import PersonalInfo from "../pages/PersonalInfo";

const [Nav, setNav] =  createSignal(<span class={"flex justify-center"}> Hello Stranger !! </span>);
const all = [
  Login(),
  Home(),
  PersonalInfo(),
  <span class={"flex justify-center"}> Hello Stranger !! </span>,
];


const anim = a => setNav(a)
//   <Motion.div class="z-0"
//     animate={{opacity: [0, 1]}}
//     transition={{duration: 1, easing: "ease-in-out"}}
//   >{a}</Motion.div>
// );


function navigate(_){
  console.log(_);
  if (_ === '') anim(all[0]);
  else if (_ === 'home') anim(all[1]);
  else if (_ === 'personal-info') anim(all[2]);
  else if (_ === 'archive') anim(all[3]);
}


export default navigate;
export { Nav };

