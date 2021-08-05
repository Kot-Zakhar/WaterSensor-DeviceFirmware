<template>
  <div>
    <v-container>
      <v-card>
        <v-card-title class="text-h5">Current mode</v-card-title>
        <v-card-text>
          <v-radio-group
            v-model="currentMode"
            v-on:change="changeMode"
          >
            <v-radio
              v-for="mode in modes"
              :key="mode.value"
              :label="mode.label"
              :value="mode.value"
              :disabled="mode.disabled"
            ></v-radio>
          </v-radio-group>
        </v-card-text>
      </v-card>
    </v-container>

    <v-container>
      <v-card>
        <v-card-title>Restart</v-card-title>
        <v-card-actions>
          <v-btn
            color="primary"
            class="mx-auto"
            @click="sendRestart"
          >
            <v-icon left>{{ mdiRestart }}</v-icon>
            Restart
          </v-btn>
        </v-card-actions>
      </v-card>
    </v-container>
  </div>
</template>

<script>
import { mdiRestart } from '@mdi/js'

export default {
  name: "Home",
  data() {
    return {
      mdiRestart,
      modes: [
        {
          value: "bt",
          label: "Bluetooth terminal",
          disabled: false,
        },
        {
          value: "wifi-hotspot",
          label: "WiFi hotspot",
          disabled: false,
        },
        {
          value: "wifi-always",
          label: "WiFi: stay connected",
          disabled: false
        },
        {
          value: "wifi-notific-only",
          label: "WiFi: connect only for notifications",
          disabled: false
        },
      ],
      currentMode: ""
    };
  },
  created() {
    this.fetchMode();
  },
  methods: {
    async fetchMode() {
      const response = await fetch(this.$api + "/api/mode", {method: "GET"});
      const data = await response.json();

      if (data.status == "OK") {
        this.currentMode = data.payload;
      }
    },
    async changeMode(mode) {
      const reqOptions = {
        method: "POST",
        headers: {
          "Content-Type": "application/json"
        },
        body: JSON.stringify(mode)
      }
      const response = await fetch(this.$api + "/api/mode", reqOptions);
      const data = await response.json();

      if (data.status == "OK") {
        console.log("Restarted successfully.");
      }
    },
    async sendRestart() {
      await fetch(this.$api + "/api/restart", {method: "GET"});
    }
  }
};
</script>
