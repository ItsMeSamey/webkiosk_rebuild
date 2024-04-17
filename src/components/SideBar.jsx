'use strict';
import { For, createSignal, untrack } from "solid-js";
import { navigate } from '../helpers/state'
import logo from '../assets/logo.png';
import './SideBar.css'


/* @subcomponents_or_path == [{name: `string`, path: `string`}, ...] 
 *                           || path `string`
*/
function SideBarComponent(name, icon, subcomponents_or_path){
  if (typeof(subcomponents_or_path) == "string"){
    return {name: name, icon: icon, path: subcomponents_or_path}
  }
  else if (typeof(subcomponents_or_path) == "object"){
    return {name: name, icon: icon, subcomponents: subcomponents_or_path}
  }
}

/* component is return value of SideBarComponent */
function SideBarMaker(component) {
  const cls = `font-[750] will-change-transform duration-300 active:scale-95 flex items-center text-white hover:bg-red-950`;
  if (component.subcomponents === undefined){
    return (
      <li class="mb-1 group">
        <span onclick={[navigate, component.path]} class={cls + " mb-2"}>
          <span class="flex flex-row">{component.icon}{component.name}</span>
        </span>
      </li>
    );
  }
  const [visible, setVisible] = createSignal(false);
  return (
    <li class={"group"}>
      <span onclick={()=>{setVisible(!visible())}} class={cls + (visible() ? " bg-red-950":"") }>
        <span class="flex flex-row">{component.icon}{component.name}</span>
        <svg class={"will-change-auto duration-300 ml-auto size-5" + (visible() ? " fill-red-300 rotate-90":" fill-white")} width="24" height="24" viewBox="0 0 24 24"><path d="M10.707 17.707 16.414 12l-5.707-5.707-1.414 1.414L13.586 12l-4.293 4.293z"/></svg>
      </span>
      <ul class="pl-6 mt-2 transition-all duration-300 grid" style={{'grid-template-rows':visible()?'1fr':'0fr'}}>
        <div class="overflow-hidden">
          <For each={component.subcomponents}>
            {(item) => (
              <li class="will-change-transform transition ease-out delay-0 active:scale-95 duration-300 mb-4" onclick={[navigate, item.path]}>
                <span class="font-[500] text-gray-100 flex items-center hover:text-[#f84525]">{item.name}</span>{/*before:contents-[''] before:w-1 before:h-1 before:rounded-full before:bg-gray-300 before:mr-3 */} 
              </li>
            )}
          </For>
        </div>
      </ul>
    </li>
  );
};

/* build the top level sidebar component */
function SideBar(components) {
  // div class="transform-gpu z-50 bg-[#180403] p-1 sidebar-menu left-0 top-0 h-full"
  return (
    <>
      <img src={logo} class="transition-all duration-300 active:opacity-80 delay-0 ease-in-out" onclick={[navigate, '']} />
      <ul>
        <For each={components}>
          {(item)=>SideBarMaker(item)}
        </For>
      </ul>
    </>
  );
};

export default SideBar;
export { SideBarComponent };

