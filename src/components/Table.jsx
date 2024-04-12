'use strict';
import { For } from "solid-js";

function Table(heading, pairs){
  return (
    <div class={"bg-[#070707] relative overflow-x-auto rounded-xl shadow-md w-[100fr] h-auto"}>
    <table class="border-collapse rounded-3xl text-md shadow-red-50 w-full">
      <thead class="select-none font-[700] text-gray-400 uppercase bg-[#070707]">
        <tr>
          <td colspan="2" class="px-8 py-2">{heading}</td>
        </tr>
      </thead>
        <tbody class={"h-0"}>
          <For each={pairs}>
            {pair => 
              <tr class="odd:bg-[#16171b] even:bg-[#0e0b10] last:pb-2">
                <td class="select-none text-gray-300 text-left rtl:text-right pl-2 pr-4 py-1">{pair.key}</td>
                <td class={"text-center px-2 py-1 font-light subpixel-antialiased text-gray-100" + (pair.editable===true?" italic":"")}>{pair.value}</td>
              </tr>
            }
          </For>
        </tbody>
    </table>
    </div>
  );
};


export default Table;

