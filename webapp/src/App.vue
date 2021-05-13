<template>
  <v-app>

    <v-app-bar
      app
      color="primary"
    >
      <v-app-bar-nav-icon @click="drawer = !drawer"></v-app-bar-nav-icon>
      <v-toolbar-title>Water sensor</v-toolbar-title>
    </v-app-bar>

    <v-navigation-drawer
      v-model="drawer"
      absolute
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
              <v-icon vf-text="item.icon"></v-icon>
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

export default {
  name: 'App',

  data: () => ({
    drawer: false,
    pages: [
      // {
      //   text: "Home",
      //   route: "/",
      //   icon: "mdi-home"
      // },
      {
        text: "WiFi",
        route: "/wifi",
        icon: "mdi-wifi"
      },
      {
        text: "Email",
        route: "/email",
        icon: "mdi-email"
      },
      {
        text: "SIM (GSM/GPRS)",
        route: "/sim",
        icon: "mdi-sim"
      },
      {
        text: "Sensors",
        route: "/sensors",
        icon: "mdi-antenna"
      }
    ]
  }),
};
</script>
