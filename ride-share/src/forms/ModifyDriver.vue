<template>
  <v-container>
    <v-row justify="center">
      <v-col cols="12" sm="8" md="6">
        <v-card class="elevation-12">
          <v-toolbar color="primary" dark flat>
            <v-toolbar-title>Add Driver</v-toolbar-title>
          </v-toolbar>
          <v-card-text>
            <v-form>
              <v-text-field
                v-model="name"
                label="Driver First Name"
                name="firstName"
                type="text"
              />
              <v-text-field
                v-model="name"
                label="Driver Last Name"
                name="lastName"
                type="text"
              />
              <v-text-field
                v-model="phone"
                label="Driver Phone Number"
                name="phone"
                type="text"
              />
              <v-text-field
                v-model="licenseNumber"
                label="License Number"
                name="licenseNumber"
                type="text"
              />
              <v-text-field
                v-model="licenseState"
                label="License State (e.g. 'AZ', 'IN', 'NY')"
                name="LicenseState"
                type="text"
              />
            </v-form>
          </v-card-text>
          <v-card-actions>
            <v-spacer />
            <v-btn v-on:click="modifyDriver" color="primary">Modify</v-btn>
          </v-card-actions>
        </v-card>
      </v-col>
    </v-row>

    <v-snackbar v-model="snackbar.show">
      {{ snackbar.msge }}
      <v-btn text color="primary" @click="snackbar.show = false">Close</v-btn>
    </v-snackbar>
  </v-container>
</template>

<script>
export default {
  data() {
    return {
      email: "",
      password: "",

      snackbar: {
        show: false,
        msge: "",
      },
    };
  },

  methods: {

    modifyDriver() {
      this.$alert("Driver Modified", "Success", "success")
      this.$router.push({ name: "admin-page" });
    },

    showSnackbar(msge) {
      this.snackbar.msge = msge;
      this.snackbar.show = true;
    },
  },
};
</script>
