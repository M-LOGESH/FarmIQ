import { defineConfig } from 'vite'
import tailwindcss from '@tailwindcss/vite'

export default defineConfig({
  plugins: [tailwindcss()],
  build: {
    rollupOptions: {
      input: {
        main: 'index.html',
        analysis: 'src/pages/analysis.html',
        irrigation: 'src/pages/irrigation.html',
        livecrop: 'src/pages/livecrop.html',
        customize: 'src/pages/customize.html'
      }
    }
  }
})
