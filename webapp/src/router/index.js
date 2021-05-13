import Vue from 'vue'
import VueRouter from 'vue-router'
// import Home from '../views/Home.vue'
import Email from '../views/Email.vue'
import WiFi from '../views/WiFi.vue'
import Sim from '../views/Sim.vue'
import Sensors from '../views/Sensors.vue'

Vue.use(VueRouter)

const routes = [
  // {
  //   path: '/',
  //   name: 'Home',
  //   component: Home
  // },
  { path: "/", redirect: "/wifi" },
  { path: '/wifi', component: WiFi },
  { path: '/email', component: Email },
  { path: '/sim', component: Sim },
  { path: '/sensors', component: Sensors }
]

const router = new VueRouter({
  routes
})

export default router
