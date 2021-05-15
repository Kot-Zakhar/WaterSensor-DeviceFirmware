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
                class="d-flex justify-center"
              >
                <v-btn
                  type="submit"
                  color="primary"
                  :disabled="!pinValid"
                  class="mx-auto"
                >
                  <v-icon left>mdi-content-save</v-icon>
                  Save
                </v-btn>
                <v-btn
                  color="secondary"
                  @click="deletePin"
                  class="mx-auto"
                >
                  <v-icon left>mdi-delete</v-icon>
                  Delete
                </v-btn>
              </v-col>
            </v-row>
          </v-form>
        </v-card-text>
      </v-card>
    </v-container>

    <v-container>
      <v-card>
        <v-card-title class="text-h5">Gsm recipients</v-card-title>
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
                <v-btn icon @click="deleteRecipient(i)">
                  <v-icon>mdi-delete</v-icon>
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
                class="d-flex justify-center"
              >
                <v-btn
                  type="submit"
                  color="primary"
                  :disabled="!newGsmRecipientValid"
                  class="mx-auto"
                >
                  <v-icon left>mdi-content-save</v-icon>
                  Save
                </v-btn>
              </v-col>
            </v-row>
          </v-form>
        </v-card-text>
      </v-card>
    </v-container>
  </div>
</template>

<script>
export default {
  name: "Sim",
  data() {
    return {
      gsmRecipients: [],
      newGsmRecipient: "",
      newGsmRecipientValid: false,
      pin: "",
      pinValid: false,
      rules: {
        required: v => !!v || "This field is required.",
        phone: v => /^\+\d{12}$/.test(v) || "Insert proper phone number (e.g. +375121234567).",
        pin: v => /^[0-9]{4,8}$/.test(v) || "Pin must contain from 4 to 8 digits."
      }
    }
  },
  created() {
    this.fetchGsmResipients();
  },
  methods: {
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
    }
  }
};
</script>

<style>
</style>