'use strict';
import { render } from 'solid-js/web';
import { Match, Show, Switch } from "solid-js";
import {Motion, Presence} from 'solid-motionone';

import {nav, bar, sidebar, navigate} from './helpers/state';
import setup from './helpers/setup.js';
import './assets/index.css';
import './helpers/parser.js';

import DefaultBar from './components/SideBarBuilder';
import Login from './pages/Login';
import Home from './pages/Home';
import PersonalInfo from "./pages/PersonalInfo";

const root = document.getElementById('root');
if (import.meta.env.DEV && !(root instanceof HTMLElement)) {
  throw new Error('Root element not found !!');
}

render(()=>{
  setup();
  navigate('home');
  sidebar(true);
  const bar_memo = DefaultBar();
  // <div class='max-[715px]:hidden sidebar-menu max-[715px]:w-0 transition-all duration-500' />
  const Anim = x => <Motion class={"h-full"+(bar()?" min-[715px]:ml-56":"")} transition={{duration: .5, easing: "ease-in-out"}} animate={{opacity: [0, 1]}} exit={{opacity:[1,.25], scale: [1,.65], transition: {duration: 0.2}}}>{x.children}</Motion>;
  return (
    <>
      <Presence>
        <Show when={bar()}>
          <Motion.div
            class={"fixed top-0 left-0 bg-black/75 z-[40] min-[715px]:hidden w-full h-full"}
            style={{"backdrop-filter":"blur(8px)"}}
            onclick={()=>{sidebar(false);setTimeout(()=>sidebar(true),1000);}}// TODO:FIX!!
            transition={{duration: .75, easing: "ease-in-out"}}
            animate={{opacity: [0, 1]}}
            exit={{ opacity: [1, 0], transition: {duration: .2}}} 
          />
        </Show>
      </Presence>
      <div class='h-full w-full'>
        <Presence>
          <Show when={bar()}>
            <Motion.aside
              class={"touch:hidden select-none overflow-scroll fixed z-50 bg-[#180403] p-1 sidebar-menu left-0 top-0 min-h-full h-full min-w-32"} 
              transition={{duration: .75, easing: "ease-in-out"}} 
              animate={{x: ['-22rem', 0]}}
              exit={{ x: [0, '-22rem'], transition: {duration: .2}}}
            >
              {bar_memo}
            </Motion.aside>
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


