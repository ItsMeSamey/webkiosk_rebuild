'use default';

export default function(){
  globalThis.__ = {};
  let x = window.outerWidth;
  let y = window.outerHeight;
  let px = x/2;
  let py = y/2;


  let hoverable;
  root.appendChild(
    <div ref={hoverable}
      id="background-glow"
      class="fixed rounded-full w-[100vmax] h-[100vmax] z-[-1] left-[-50vmax] top-[-50vmax]" // 
    />
  );
  const h_style = hoverable.style

  setInterval(()=>{
    h_style.cssText = `transform: translate(${px}px,${py}px);`;
  },100/3)

  window.onmousemove = e => {
    px = e.clientX;
    py = e.clientY;
  }
  window.onresize = () => {
    // workaround when unmaxamizing window as normally setting values causes wrong values to be set (values before resize are set);
    setTimeout(()=>{
      x = window.outerWidth;
      y = window.outerHeight;
    },0);
  }
};

