<template>
  <div>
    <v-container>
      <v-card>
        <v-card-title class="text-h5">Email recipients</v-card-title>
        <v-card-text>
          <v-card-subtitle v-if="!emailRecipients.length">No recipients. Add one below.</v-card-subtitle>
          <v-list>
            <v-list-item
              v-for="(rec, i) in emailRecipients"
              :key="i">
              <v-list-item-content>
                <v-list-item-title v-text="rec" class="text-subtitle-1"></v-list-item-title>
              </v-list-item-content>
              <v-list-item-action>
                <v-btn icon @click="deleteEmailRecipient(i)">
                  <v-icon>{{ mdi.mdiDelete }}</v-icon>
                </v-btn>
              </v-list-item-action>
            </v-list-item>
          </v-list>
          <v-form
            v-on:submit.prevent="saveEmailRecipient"
            v-model="newEmailRecipientValid"
            ref="newEmailRecipientForm"
          >
            <v-row>
              <v-col cols="12" md="6">
                <v-text-field
                  v-model="newEmailRecipient"
                  label="Email"
                  :rules="[rules.required, rules.email]"
                  clearable
                ></v-text-field>
              </v-col>
              <v-col cols="12" md="6" align-self="center" class="d-flex align-center justify-center">
                <v-btn
                  block
                  type="submit"
                  color="primary"
                  :disabled="!newEmailRecipientValid"
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
        <v-card-title class="text-h5">SMTP credentials</v-card-title>
        <v-card-text>
          <v-form
            v-on:submit.prevent="saveSmtpSettings"
            v-model="smtpSettingsValid"
            ref="smtpSettingsForm"
          >
            <v-text-field
              v-model="smtpSettings.server"
              label="Server"
              :rules="[rules.required]"
              clearable
            ></v-text-field>
            <v-text-field
              v-model="smtpSettings.port"
              label="Port"
              :rules="[rules.required, rules.digits]"
              clearable
            ></v-text-field>
            <v-text-field
              v-model="smtpSettings.login"
              label="Login"
              :rules="[rules.required]"
              clearable
            ></v-text-field>
            <v-text-field
              v-model="smtpSettings.password"
              label="Password"
              :rules="[rules.required]"
              :append-icon="showSmtpPasswordVisible ? mdi.mdiEye : mdi.mdiEyeOff"
              :type="showSmtpPasswordVisible ? 'text' : 'password'"
              @click:append="showSmtpPasswordVisible = !showSmtpPasswordVisible"
              clearable
            ></v-text-field>
            <v-switch
              v-model="smtpSettings.ssl"
              label="Use SSL"
            ></v-switch>
            <v-card-actions>

              <v-row>
                <v-col col="6" md="3">
                  <v-btn block color="primary" type="submit" :disabled="!smtpSettingsValid" class="mr-auto">
                    <v-icon left>{{ mdi.mdiContentSave }}</v-icon>
                    Save
                  </v-btn>
                </v-col>
                <v-col col="6" md="3">
                  <v-btn block color="primary" @click="fetchSmtpSettings" class="mx-auto">
                    <v-icon left>{{ mdi.mdiDownload }}</v-icon>
                    Fetch
                  </v-btn>
                </v-col>
                <v-col col="6" md="3">
                  <v-btn block @click="clearForm('smtpSettingsForm')" class="mx-auto">
                    <v-icon left>{{ mdi.mdiClose }}</v-icon>
                    Clear
                  </v-btn>
                </v-col>
                <v-col col="6" md="3">
                  <v-btn block color="secondary" @click="deleteSmtp" class="ml-auto">
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
      <v-btn block @click="sendTestEmailWifi" class="mx-auto">
        <v-icon left>{{ mdi.mdiEmail }}</v-icon>
        Send test email using Wi-Fi
      </v-btn>
    </v-container>

    <v-container>
      <v-card>
        <v-card-title class="text-h5">IMAP credentials</v-card-title>
        <v-card-text>
          <v-form
            v-on:submit.prevent="saveImapSettings"
            v-model="imapSettingsValid"
            ref="imapSettingsForm"
          >
            <v-text-field
              v-model="imapSettings.server"
              label="Server"
              :rules="[rules.required]"
              clearable
            ></v-text-field>
            <v-text-field
              v-model="imapSettings.port"
              label="Port"
              :rules="[rules.required, rules.digits]"
              clearable
            ></v-text-field>
            <v-text-field
              v-model="imapSettings.login"
              label="Login"
              :rules="[rules.required]"
              clearable
            ></v-text-field>
            <v-text-field
              v-model="imapSettings.password"
              label="Password"
              :rules="[rules.required]"
              :append-icon="showImapPasswordVisible ? mdi.mdiEye : mdi.mdiEyeOff"
              :type="showImapPasswordVisible ? 'text' : 'password'"
              @click:append="showImapPasswordVisible = !showImapPasswordVisible"
              clearable
            ></v-text-field>
            <v-switch
              v-model="imapSettings.ssl"
              label="Use SSL"
            ></v-switch>
            <v-card-actions>
              <v-row>
                <v-col col="6" md="3">
                  <v-btn block color="primary" type="submit" :disabled="!imapSettingsValid" class="mr-auto">
                    <v-icon left>{{ mdi.mdiContentSave }}</v-icon>
                    Save
                  </v-btn>
                </v-col>
                <v-col col="6" md="3">
                  <v-btn block color="primary" @click="fetchImapSettings" class="mx-auto">
                    <v-icon left>{{ mdi.mdiDownload }}</v-icon>
                    Fetch
                  </v-btn>
                </v-col>
                <v-col col="6" md="3">
                  <v-btn block @click="clearForm('imapSettingsForm')" class="mx-auto">
                    <v-icon left>{{ mdi.mdiClose }}</v-icon>
                    Clear
                  </v-btn>
                </v-col>
                <v-col col="6" md="3">
                  <v-btn block color="secondary" @click="deleteImap" class="ml-auto">
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
  </div>
</template>

<script>
import {
  mdiContentSave,
  mdiEye,
  mdiEyeOff,
  mdiDownload,
  mdiClose,
  mdiDelete,
  mdiEmail
} from '@mdi/js'

export default {
  name: "Email",
  data() {
    return {
      mdi: {
        mdiContentSave,
        mdiEye,
        mdiEyeOff,
        mdiDownload,
        mdiClose,
        mdiDelete,
        mdiEmail
      },
      smtpSettingsValid: false,
      showSmtpPasswordVisible: false,
      smtpSettings: {
        server: "",
        port: "",
        login: "",
        password: "",
        ssl: false
      },
      imapSettingsValid: false,
      showImapPasswordVisible: false,
      imapSettings: {
        server: "",
        port: "",
        login: "",
        password: "",
        ssl: false
      },
      emailRecipients: [],
      newEmailRecipient: "",
      newEmailRecipientValid: false,
      rules: {
        required: v => !!v || "This field is required.",
        email: v => /^.+@.+$/.test(v) || "Insert proper email address.",
        digits: v => /^[0-9]+$/.test(v) || "Only numbers are allowed."
      }
    }
  },
  created() {
    this.fetchSmtpSettings();
    this.fetchImapSettings();
    this.fetchEmailResipients();
  },
  methods: {
    clearForm(formName) {
      this.$refs[formName].reset();
    },
    async deleteSmtp() {
      const reqOptions = {
        method: "DELETE"
      }
      const res = await fetch(this.$api + "/api/email-smtp", reqOptions);
      const data = await res.json();

      if (data.status == "OK") {
        this.clearForm("smtpSettingsForm");
      }
    },
    async fetchSmtpSettings() {
      const response = await fetch(this.$api + "/api/email-smtp", {method: "GET"});
      const data = await response.json();

      if (data.status == "OK" && data.payload) {
        this.smtpSettings = data.payload;
      }
    },
    async saveSmtpSettings() {
      const reqOptions = {
        method: "POST",
        headers: {
          "Content-Type": "application/json"
        },
        body: JSON.stringify(this.smtpSettings)
      }

      const res = await fetch(this.$api + "/api/email-smtp", reqOptions);
      const data = await res.json();
      if (data.status !== "OK") {
        console.error(data);
      }
    },
    async deleteImap() {
      const reqOptions = {
        method: "DELETE"
      }
      const res = await fetch(this.$api + "/api/email-imap", reqOptions);
      const data = await res.json();

      if (data.status == "OK") {
        this.clearForm("imapSettingsForm");
      }
    },
    async fetchImapSettings() {
      const response = await fetch(this.$api + "/api/email-imap", {method: "GET"});
      const data = await response.json();

      if (data.status == "OK" && data.payload) {
        this.imapSettings = data.payload;
      }
    },
    async saveImapSettings() {
      const reqOptions = {
        method: "POST",
        headers: {
          "Content-Type": "application/json"
        },
        body: JSON.stringify(this.imapSettings)
      }

      const res = await fetch(this.$api + "/api/email-imap", reqOptions);
      const data = await res.json();
      if (data.status !== "OK") {
        console.error(data);
      }
    },
    async fetchEmailResipients() {
      const reqOptions = {
        method: "GET",
        headers: {
          "Accept": "application/json"
        },
      }
      const res = await fetch(this.$api + "/api/email-recipients", reqOptions);
      const data = await res.json();
      if (data.status == "OK") {
        this.emailRecipients = data.payload;
      } else {
        console.error(data);
      }
    },
    async saveEmailRecipient() {
      const reqOptions = {
        method: "POST",
        headers: {
          "Content-Type": "application/json"
        },
        body: JSON.stringify(this.newEmailRecipient)
      }

      const res = await fetch(this.$api + "/api/email-recipients", reqOptions);
      const data = await res.json();
      if (data.status !== "OK") {
        console.error(data)
      }
      this.clearForm("newEmailRecipientForm");

      await this.fetchEmailResipients();
    },
    async deleteEmailRecipient(index) {
      const res = await fetch(this.$api + "/api/email-recipients/" + index, {method: "DELETE"});
      const data = await res.json();
      if (data.status !== "OK") {
        console.error(data)
      }
      await this.fetchEmailResipients();
    },
    async sendTestEmailWifi() {
      const res = await fetch(this.$api + "/api/email-test", {method: "GET"});
      const data = await res.json();
      console.log(data);
    }
  }
};
</script>

<style>
</style>