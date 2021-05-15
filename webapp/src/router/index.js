import Vue from 'vue'
import VueRouter from 'vue-router'
import Home from '../views/Home.vue'
import Email from '../views/Email.vue'
import WiFi from '../views/WiFi.vue'
import Sim from '../views/Sim.vue'
import Sensors from '../views/Sensors.vue'

Vue.use(VueRouter)

const routes = [
  {
    name: 'Home',
    path: '/',
    component: Home
  },
  // { path: "/", redirect: "/wifi" },
  {
    name: "WiFi",
    path: '/wifi',
    component: WiFi
  },
  {
    name: "Email",
    path: '/email',
    component: Email
  },
  {
    name: "SIM (GSM/GPRS)",
    path: '/sim',
    component: Sim
  },
  {
    name: "Sensors",
    path: '/sensors',
    component: Sensors
   }
]

const router = new VueRouter({
  routes
})

export default router
