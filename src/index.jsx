'use strict';
import { render } from 'solid-js/web';

import DefaultBar from './components/SideBarBuilder';
import { Nav } from './helpers/Navigator';
import './assets/index.css';

const root = document.getElementById('root');
if (import.meta.env.DEV && !(root instanceof HTMLElement)) {
  throw new Error('Root element not found !!');
}

render(() => (
  <>
    <div class='flex flex-auto text-white bg-[#1f1010] h-[100%] min-h[100vh] w-[100%]'>
      <div class='max-[715px]:hidden sidebar-menu'></div>
      {Nav()}
    </div>
    <DefaultBar/>
  </>

), root);

