'use strict';

import { createSignal } from "solid-js";
import { Motion } from "solid-motionone";

export default function(){
const [bg, setBg] = createSignal("red")
  return (
    <>
      <div class="fixed top-0 left-0 w-[101%] h-full bg-black z-[100] blur-sm" style={{'backdrop-filter': 'blur(10px)',}} />
      <div class="absolute h-full w-full z-[101]">
        <span> OKAY </span>
      </div>
      <Motion.button
        onClick={() => setBg("blue")}
        animate={{backgroundColor: bg()}}
        transition={{duration: 3}}
      >
        Click Me
      </Motion.button>
    </>
  );
}

