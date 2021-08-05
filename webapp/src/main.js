import Vue from 'vue'
import App from './App.vue'
import router from './router'
import vuetify from './plugins/vuetify'
import VueApexCharts from 'vue-apexcharts'

Vue.config.productionTip = false;

Vue.prototype.$api = process.env.VUE_APP_API_ADDRESS || "";

Vue.use(VueApexCharts)

new Vue({
  router,
  vuetify,
  render: h => h(App)
}).$mount('#app')
