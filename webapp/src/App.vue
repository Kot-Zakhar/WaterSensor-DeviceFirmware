<template>
  <v-app>

    <v-app-bar app color="primary" dark>
      <v-app-bar-nav-icon @click="drawer = !drawer"><v-icon>{{ mdiMenu }}</v-icon></v-app-bar-nav-icon>
      <v-toolbar-title class="text-h5">Water sensor : {{ $route.name }}</v-toolbar-title>
    </v-app-bar>

    <v-navigation-drawer
      v-model="drawer"
      app
      bottom
      temporary
    >
      <v-list
        nav
        dense
      >
        <v-list-item-group>
          <v-list-item
            v-for="(item, i) in pages"
            :key="i"
            :to="item.route">
            <v-list-item-icon>
              <v-icon v-text="item.icon"></v-icon>
            </v-list-item-icon>
            <v-list-item-content>
              <v-list-item-title v-text="item.text"></v-list-item-title>
            </v-list-item-content>
          </v-list-item>
        </v-list-item-group>
      </v-list>
    </v-navigation-drawer>

    <v-main>
      <router-view/>
    </v-main>
  </v-app>
</template>

<script>

import { mdiHome, mdiWifi, mdiEmail, mdiSim, mdiAntenna, mdiMenu } from '@mdi/js'

export default {
  name: 'App',

  data: () => ({
    mdiMenu,
    drawer: false,
    pages: [
      {
        text: "Home",
        route: "/",
        icon: mdiHome
      },
      {
        text: "WiFi",
        route: "/wifi",
        icon: mdiWifi
      },
      {
        text: "Email",
        route: "/email",
        icon: mdiEmail
      },
      {
        text: "SIM (GSM/GPRS)",
        route: "/sim",
        icon: mdiSim
      },
      {
        text: "Sensors",
        route: "/sensors",
        icon: mdiAntenna
      }
    ]
  })
};
</script>
