'use strict';
import { render } from 'solid-js/web';
import { Match, Switch, getOwner, runWithOwner } from "solid-js";
import {Motion, Presence} from 'solid-motionone'

import {nav, bar} from './helpers/state';
import DefaultBar from './components/SideBarBuilder';
import Login from './pages/Login';
import Home from './pages/Home';
import PersonalInfo from "./pages/PersonalInfo";
import './assets/index.css';
import './helpers/parser.js'

const root = document.getElementById('root');
if (import.meta.env.DEV && !(root instanceof HTMLElement)) {
  throw new Error('Root element not found !!');
}


render(()=>{
  const Anim = (x) => <Motion transition={{duration: .5, easing: "ease-in-out"}} animate={{opacity: [0, 1]}} exit={{opacity:[1,.25], scale: [1,.65], transition: {duration: 0.2}}}>{x.children}</Motion>;
  return (
    <>
      <div class='flex flex-row bg-[#1f1010] h-full w-full left-0 top-0 '>
        <div class="fixed top-0 left-0 w-full h-full bg-black/75 z-[40] min-[715px]:hidden" />
        <Presence exitBeforeEnter>
          <Show when={bar()}>
            <Motion.div class="transform-gpu z-50 bg-[#180403] p-1 sidebar-menu left-0 top-0 h-full" transition={{duration: 1, easing: "ease-in-out"}} animate={{x: ['-22rem', 0]}} exit={{ transition: {duration: 0.4}}} >
              <DefaultBar/>
            </Motion.div>
          </Show>
        </Presence>
        <Presence exitBeforeEnter>
          <Switch>
            <Match when={nav() === ''}>
              <Anim><Login/></Anim>
            </Match>
            <Match when={nav() === 'home'}>
              <Anim><Home/></Anim>
            </Match>
            <Match when={nav() === 'personal-info'}>
              <Anim><PersonalInfo/></Anim>
            </Match>
            <Match when={nav() === 'archive'}>
              <Anim><span class={"flex justify-center"}> Hello Stranger !! </span></Anim>
            </Match>
          </Switch>
        </Presence>
      </div>
    </>
  );
}, root);

      // <div class='max-[715px]:hidden sidebar-menu max-[715px]:w-0 transition-all duration-500' />

