<template>
  <v-container>
    <v-card
      :loading="loading"
      class="m-12"
    >
      <template slot="progress">
        <v-progress-linear></v-progress-linear>
      </template>

      <v-card-title>Wi-Fi Settings</v-card-title>

      <v-divider class="mx-4"></v-divider>

      <v-card-subtitle class="text-h5">Wi-Fi credentials</v-card-subtitle>
      <v-card-text>
        <v-list>
          <v-list-item
            v-for="(record, i) in wifiRecords"
            :key="i">
              <v-list-item-content>
                <v-list-item-title v-text="record.ssid" class="text-subtitle-1"></v-list-item-title>
                <!-- <v-list-item-subtitle v-text="record.password"></v-list-item-subtitle> -->
              </v-list-item-content>
              <v-list-item-action>
                <v-btn icon @click="deleteWifiRecord(i)">
                  <v-icon>mdi-delete</v-icon>
                </v-btn>
              </v-list-item-action>
          </v-list-item>
        </v-list>
        <v-form
          v-on:submit="saveRecord"
          v-model="newWifiRecordValid"
          ref="newWifiRecordForm"
        >
          <v-row>
            <v-col cols="12" md="4">
              <v-text-field
                v-model="newWifiRecord.ssid"
                label="SSID"
                :rules="[rules.required]"
                ></v-text-field>
            </v-col>
            <v-col cols="12" md="4">
              <v-text-field
                v-model="newWifiRecord.password"
                label="Password"
                :rules="[rules.required]"
                ></v-text-field>
            </v-col>
            <v-col cols="12" md="4" align-self="center" class="d-flex justify-center">
              <v-btn class="mx-auto"
                type="submit"
                :disabled="!newWifiRecordValid"
              >
                submit
              </v-btn>
              <v-btn @click="clear" class="mx-auto">
                clear
              </v-btn>
            </v-col>
          </v-row> </v-form>
      </v-card-text>
    </v-card>
  </v-container>
</template>

<script>
export default {
  name: 'WiFi',
  data () {
    return {
      loading: true,
      wifiRecords: [],
      newWifiRecordValid: false,
      newWifiRecord: {
        ssid: '',
        password: ''
      },
      rules: {
        required: v => !!v || "This field is required."
      }
    }
  },
  created () {
    this.fetchRecords()
  },
  methods: {
    clear() {
      this.$refs.newWifiRecordForm.reset();
    },
    async fetchRecords () {
      this.loading = true
      
      const reqOptions = {
        method: 'GET'
      }

      const response = await fetch(this.$api + "/api/wifi-creds", reqOptions);
      const data = await response.json();

      if (data.status == "OK") {
        this.wifiRecords = data.payload;
      }
    },
    async saveRecord () {
      const reqOptions = {
        method: "POST",
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(this.newWifiRecord)
      }

      const res = await fetch(this.$api + "/api/wifi-creds", reqOptions);
      const data = await res.json();

      if (data.status == "OK") {
        this.clear()
        this.fetchRecords()
      }
    },
    async deleteWifiRecord(recordIndex) {
      const reqOption = {
        method: "DELETE",
        headers: {
          'Content-Type': 'application/json'
        }
      }

      const res = await fetch(this.$api + "/api/wifi-creds/" + recordIndex, reqOption);
      const data = await res.json();

      if (data.status == "OK") {
        this.fetchRecords()
      }
    }
  }
  
}
</script>

<style>

</style>