'use strict';

import { Motion, Presence } from "solid-motionone";

import { bar } from '../helpers/state.jsx';
import { Show } from "solid-js";

export default function(){
  return (
    <Presence>
      <Show when={bar()}>
        <Motion.aside
          class={"float select-none overflow-clip transform-gpu z-[49] left-0 top-0 size-10 rounded-full"} 
          transition={{duration: .75, easing: "ease-in-out"}} 
          animate={{rotate: [0, 1]}}
          exit={{ x: [0, '-22rem'], transition: {duration: .2}}}
        >
        </Motion.aside>
      </Show>
    </Presence>
  );
}

