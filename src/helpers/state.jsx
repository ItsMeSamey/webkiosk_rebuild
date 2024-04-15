'use strict';

import { createSignal } from "solid-js";

// const anim = a => <Motion.div class="z-0" animate={{opacity: [0, 1]}} transition={{duration: 1, easing: "ease-in-out"}}>{a}</Motion.div>

const [nav, navigate] = createSignal('');
const [bar, sidebar] = createSignal(true);

export { nav, navigate, bar, sidebar };

