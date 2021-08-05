<template>
  <v-container>
    <v-card>
      <v-card-title class="text-h5">Wi-Fi credentials</v-card-title>
      <v-card-text>
        <v-card-subtitle v-if="!wifiRecords.length">No Wi-Fi records. Add one below</v-card-subtitle>
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
                  <v-icon>{{ mdi.mdiDelete }}</v-icon>
                </v-btn>
              </v-list-item-action>
          </v-list-item>
        </v-list>
        <v-form
          v-on:submit.prevent="saveRecord"
          v-model="newWifiRecordValid"
          ref="newWifiRecordForm"
        >
          <v-row>
            <v-col cols="12" md="4">
              <v-text-field
                v-model="newWifiRecord.ssid"
                label="SSID"
                :rules="[rules.required]"
                clearable
                ></v-text-field>
            </v-col>
            <v-col cols="12" md="4">
              <v-text-field
                v-model="newWifiRecord.password"
                label="Password"
                :rules="[rules.required]"
                :append-icon="newWifiRecordPasswordVisible ? mdi.mdiEye : mdi.mdiEyeOff"
                :type="newWifiRecordPasswordVisible ? 'text' : 'password'"
                @click:append="newWifiRecordPasswordVisible = !newWifiRecordPasswordVisible"
                clearable
                ></v-text-field>
            </v-col>
            <v-col cols="12" md="4" align-self="center" class="d-flex align-center">
              <v-row>
                <v-col col="6">
                  <v-btn block
                    type="submit"
                    color="primary"
                    :disabled="!newWifiRecordValid"
                    class="mx-auto"
                  >
                    <v-icon left>{{ mdi.mdiContentSave }}</v-icon>
                    Save
                  </v-btn>
                </v-col>
                <v-col col="6">
                  <v-btn block @click="clear" class="mx-auto">
                    <v-icon left>{{ mdi.mdiClose }}</v-icon>
                    Clear
                  </v-btn>
                </v-col>
              </v-row>
            </v-col>
          </v-row> </v-form>
      </v-card-text>
    </v-card>
  </v-container>
</template>

<script>
import {
  mdiDelete,
  mdiEye,
  mdiEyeOff,
  mdiContentSave,
  mdiClose
} from '@mdi/js'

export default {
  name: 'WiFi',
  data () {
    return {
      mdi: {
        mdiDelete,
        mdiEye,
        mdiEyeOff,
        mdiContentSave,
        mdiClose
      },
      loading: true,
      wifiRecords: [],
      newWifiRecordValid: false,
      newWifiRecordPasswordVisible: false,
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
        method: "DELETE"
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