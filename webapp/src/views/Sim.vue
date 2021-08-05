<template>
  <div>
    <v-container>
      <v-card>
        <v-card-title class="text-h5">Sim PIN</v-card-title>
        <v-card-text>
          <v-form
            v-on:submit.prevent="savePin"
            v-model="pinValid"
            ref="pinForm"
          >
            <v-row>
              <v-col cols="12" md="6">
                <v-text-field
                  v-model="pin"
                  label="PIN"
                  :rules="[rules.required, rules.pin]"
                  clearable
                ></v-text-field>
              </v-col>
              <v-col
                cols="12"
                md="6"
                class="d-flex align-center"
              >
                <v-row>
                  <v-col col="6">
                    <v-btn
                      block
                      type="submit"
                      color="primary"
                      :disabled="!pinValid"
                      class="mx-auto"
                    >
                      <v-icon left>{{ mdi.mdiContentSave }}</v-icon>
                      Save
                    </v-btn>
                  </v-col>
                  <v-col col="6">
                    <v-btn
                      block
                      color="secondary"
                      @click="deletePin"
                      class="mx-auto"
                    >
                      <v-icon left>{{ mdi.mdiDelete }}</v-icon>
                      Delete
                    </v-btn>
                  </v-col>
                </v-row>
              </v-col>
            </v-row>
          </v-form>
        </v-card-text>
      </v-card>
    </v-container>

    <v-container>
      <v-card>
        <v-card-title class="text-h5">GSM recipients</v-card-title>
        <v-card-text>
          <v-card-subtitle v-if="!gsmRecipients.length"
            >No recipients. Add one below.</v-card-subtitle
          >
          <v-list>
            <v-list-item v-for="(rec, i) in gsmRecipients" :key="i">
              <v-list-item-content>
                <v-list-item-title
                  v-text="rec"
                  class="text-subtitle-1"
                ></v-list-item-title>
              </v-list-item-content>
              <v-list-item-action>
                <v-btn icon @click="deleteGsmRecipient(i)">
                  <v-icon>{{ mdi.mdiDelete }}</v-icon>
                </v-btn>
              </v-list-item-action>
            </v-list-item>
          </v-list>
          <v-form
            v-on:submit.prevent="saveGsmRecipient"
            v-model="newGsmRecipientValid"
            ref="newGsmRecipientForm"
          >
            <v-row>
              <v-col cols="12" md="6">
                <v-text-field
                  v-model="newGsmRecipient"
                  label="Phone"
                  :rules="[rules.required, rules.phone]"
                  clearable
                ></v-text-field>
              </v-col>
              <v-col
                cols="12"
                md="6"
                class="d-flex align-center"
              >
                <v-btn
                  block
                  type="submit"
                  color="primary"
                  :disabled="!newGsmRecipientValid"
                  class="mx-auto"
                >
                  <v-icon left>{{ mdi.mdiContentSave }}</v-icon>
                  Save
                </v-btn>
              </v-col>
            </v-row>
          </v-form>
        </v-card-text>
      </v-card>
    </v-container>

    <v-container>
      <v-card>
        <v-card-title class="text-h5">GPRS settings</v-card-title>
        <v-card-text>
          <v-switch
            v-model="allowGprs"
            v-on:change="saveGprsPermission"
            label="Allow the use of mobile network (GPRS)"
          ></v-switch>

          <v-form
            v-on:submit.prevent="saveGprsSettings"
            v-model="gprsSettingsValid"
            ref="gprsSettingsForm"
          >
            <v-text-field
              v-model="gprsSettings.apn"
              label="Apn"
              :rules="[rules.required]"
              clearable
            ></v-text-field>
            <v-text-field
              v-model="gprsSettings.user"
              label="User"
              :rules="[rules.required]"
              clearable
            ></v-text-field>
            <v-text-field
              v-model="gprsSettings.password"
              label="Password"
              :rules="[rules.required]"
              clearable
            ></v-text-field>
            <v-card-actions>
              <v-row>
                <v-col col="6" md="3">
                  <v-btn block color="primary" type="submit" :disabled="!gprsSettingsValid" class="mr-auto">
                    <v-icon left>{{ mdi.mdiContentSave }}</v-icon>
                    Save
                  </v-btn>
                </v-col>
                <v-col col="6" md="3">
                  <v-btn block color="primary" @click="fetchGprsSettings" class="mx-auto">
                    <v-icon left>{{ mdi.mdiDownload }}</v-icon>
                    Fetch
                  </v-btn>
                </v-col>
                <v-col col="6" md="3">
                  <v-btn block @click="clearForm('gprsSettingsForm')" class="mx-auto">
                    <v-icon left>{{ mdi.mdiClose }}</v-icon>
                    Clear
                  </v-btn>
                </v-col>
                <v-col col="6" md="3">
                  <v-btn block color="secondary" @click="deleteGprsSettings" class="ml-auto">
                    <v-icon left>{{ mdi.mdiDelete }}</v-icon>
                    Delete
                  </v-btn>
                </v-col>
              </v-row>
            </v-card-actions>
          </v-form>
        </v-card-text>
      </v-card>
    </v-container>
    <v-container>
      <v-btn block @click="sendTestSms" class="mx-auto">
        <v-icon left>{{ mdi.mdiEmail }}</v-icon>
        Send test SMS
      </v-btn>
    </v-container>
    <v-container>
      <v-btn block @click="sendTestEmailGprs" class="mx-auto">
        <v-icon left>{{ mdi.mdiEmail }}</v-icon>
        Send test email using GPRS
      </v-btn>
    </v-container>
  </div>
</template>

<script>
import {
  mdiContentSave,
  mdiDelete,
  mdiDownload,
  mdiClose,
  mdiEmail
} from '@mdi/js'

export default {
  name: "Sim",
  data() {
    return {
      mdi: {
        mdiContentSave,
        mdiDelete,
        mdiDownload,
        mdiClose,
        mdiEmail
      },
      gsmRecipients: [],
      newGsmRecipient: "",
      newGsmRecipientValid: false,
      allowGprs: false,
      pin: "",
      pinValid: false,
      gprsSettings: {
        apn: "",
        user: "",
        password: ""
      },
      gprsSettingsValid: false,
      rules: {
        required: v => !!v || "This field is required.",
        phone: v => /^\+\d{12}$/.test(v) || "Insert proper phone number (e.g. +375121234567).",
        pin: v => /^[0-9]{4,8}$/.test(v) || "Pin must contain from 4 to 8 digits."
      }
    }
  },
  created() {
    this.fetchGsmResipients();
    this.fetchGprsPermission();
    this.fetchGprsSettings();
  },
  methods: {
    clearForm(formName) {
      this.$refs[formName].reset();
    },
    async fetchGsmResipients() {
      const reqOptions = {
        method: "GET",
        headers: { "Accept": "application/json" }
      }
      const res = await fetch(this.$api + "/api/gsm-recipients", reqOptions);
      const data = await res.json();
      if (data.status == "OK") {
        this.gsmRecipients = data.payload;
      } else {
        console.error(data);
      }
    },
    async saveGsmRecipient() {
      const reqOptions = {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(this.newGsmRecipient)
      }

      const res = await fetch(this.$api + "/api/gsm-recipients", reqOptions);
      const data = await res.json();
      if (data.status !== "OK") {
        console.error(data)
      }
      this.clearForm("newGsmRecipientForm");

      await this.fetchGsmResipients();
    },
    async deleteGsmRecipient(index) {
      const res = await fetch(this.$api + "/api/gsm-recipients/" + index, {method: "DELETE"});
      const data = await res.json();
      if (data.status !== "OK") {
        console.error(data)
      }
      await this.fetchGsmResipients();
    },

    async fetchPin() {
      const res = await fetch(this.$api + "/api/gsm-pin", {method: "GET"});
      const data = await res.json();
      if (data.status == "OK") {
        this.pin = data.pyaload;
      }
    },
    async savePin() {
      const reqOptions = {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(this.pin)
      }
      const res = await fetch(this.$api + "/api/gsm-pin", reqOptions);
      const data = await res.json();
      if (data.status !== "OK") {
        console.error(data);
      }
    },
    async deletePin() {
      const res = await fetch(this.$api + "/api/gsm-pin", {method: "DELETE"});
      const data = await res.json();
      if (data.status !== "OK") {
        console.error(data);
      }
    },
    async saveGprsPermission() {
      const reqOptions = {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({allow: this.allowGprs})
      }
      const res = await fetch(this.$api + "/api/gprs-use-network-perm", reqOptions);
      const data = await res.json();
      if (data.status !== "OK") {
        console.error(data);
      }
    },
    async fetchGprsPermission() {
      const res = await fetch(this.$api + "/api/gprs-use-network-perm", {method: "GET"});
      const data = await res.json();
      if (data.status == "OK") {
        this.allowGprs = data.payload;
      }
    },
    async fetchGprsSettings() {
      const res = await fetch(this.$api + "/api/gprs", {method: "GET"});
      const data = await res.json();
      if (data.status == "OK") {
        this.gprsSettings = data.payload;
      }
    },
    async saveGprsSettings() {
      const reqOptions = {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify(this.gprsSettings)
      }
      const res = await fetch(this.$api + "/api/gprs", reqOptions);
      const data = await res.json();
      if (data.status !== "OK") {
        console.error(data);
      }
    },
    async deleteGprsSettings() {
      const res = await fetch(this.$api + "/api/gprs", {method: "DELETE"});
      const data = await res.json();
      if (data.status !== "OK") {
        console.error(data);
      }
    },
    async sendTestSms() {
      const res = await fetch(this.$api + "/api/gsm-test", {method: "GET"});
      const data = await res.json();
      console.log(data);
    },
    async sendTestEmailGprs() {
      const res = await fetch(this.$api + "/api/gprs-test", {method: "GET"});
      const data = await res.json();
      console.log(data);
    }
  }
};
</script>

<style>
</style>