'use default';



export default function(){
  globalThis.__ = {};
  globalThis.__.GLOBAL = [];
  const gradient = document.getElementsByTagName('html')[0].style;
  let x = 100/window.outerWidth;
  let y = 100/window.outerHeight;

  document.addEventListener('mousemove', e => {
    gradient.cssText = '--mouse-xp: '+x*e.screenX+'%; --mouse-yp: '+y*e.screenY+'%;';
  });
  window.addEventListener('resize', () => {
    x = 100/window.outerWidth;y = 100/window.outerHeight;
  });
};

