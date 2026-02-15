import { defineConfig } from 'vite';

export default defineConfig({
  server: {
    port: 8080,
    host: '0.0.0.0', // Allow external connections in Docker
    watch: {
      usePolling: true // Needed for Docker volume mounts
    }
  },
  build: {
    outDir: 'dist',
    sourcemap: true
  }
});

