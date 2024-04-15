import { defineConfig } from 'vite';
import { compression } from 'vite-plugin-compression2'
import solidPlugin from 'vite-plugin-solid';
// import devtools from 'solid-devtools/vite';

export default defineConfig({
  plugins: [
    compression({exclude: [/\.(br)$/, /\.(gz)$/], deleteOriginalAssets: true }),
    /* 
    Uncomment the following line to enable solid-devtools.
    For more info see https://github.com/thetarnav/solid-devtools/tree/main/packages/extension#readme
    */
    // devtools(),
    solidPlugin(),
  ],
  server: {
    cors: true,
    host: '127.0.0.1',
    port: 3000,
  },
  build: {
    target: 'esnext',
  },
});
