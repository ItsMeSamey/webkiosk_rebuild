'use strict';
import { render } from 'solid-js/web';
import { Match, Show, Suspense, Switch } from 'solid-js';
import {Motion, Presence} from 'solid-motionone';

import {nav, bar, sidebar, navigate} from './helpers/state';
import setup from './helpers/setup.js';
import DefaultBar from './components/SideBarBuilder';
import Login from './pages/Login';
import Home from './pages/Home';
import PersonalInfo from './pages/PersonalInfo';

import './assets/index.css';
import './helpers/parser.js';
import './assets/keyframes.css';

const root = document.getElementById('root');
if (import.meta.env.DEV && !(root instanceof HTMLElement)) {
  throw new Error('Root element not found !!');
}

render(()=>{
  setup();
  navigate('');
  sidebar(true);
  // <div class='max-[715px]:hidden sidebar-menu max-[715px]:w-0 transition-all duration-500' />
  const Anim = x => (
    <Motion class={'h-full'+(bar()?' min-[715px]:ml-56':'')}
      transition={{duration: .45, easing: 'ease-in-out'}}
      animate={{opacity: [0, 1]}}
      exit={{opacity:[1,.25], y: ['0','10%'], transition: {duration: 0.15}}}
    >
      {x.children}
    </Motion>
  );
  return (
    <>
      <DefaultBar/>
      <Suspense>
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
              <Anim><span class={'flex justify-center'}> Hello Stranger !! </span></Anim>
            </Match>
          </Switch>
        </Presence>
      </Suspense>
    </>
  );
}, root);


