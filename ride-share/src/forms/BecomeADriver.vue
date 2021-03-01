<template>
  <v-container>
    <v-row justify="center">
      <v-col cols="12" sm="8" md="6">
        <v-card class="elevation-12">
          <v-toolbar color="primary" dark flat>
            <v-toolbar-title>Become A Driver</v-toolbar-title>
          </v-toolbar>
          <v-card-text>
            <v-form>
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
            <v-btn v-on:click="addDriver" color="primary">Submit</v-btn>
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
    logIn() {
      this.$axios
        .post("/login", {
          email: this.email,
          password: this.password,
        })
        .then((result) => {
          this.showSnackbar(result.data.msge);
          if (result.data.ok) {
            this.$store.commit("logIn", result.data.details);
            this.$router.push({ name: "home-page" });
          }
        })
        .catch((err) => this.showSnackbar(err));
    },


    addDriver() {
      this.$alert("Driver Application Submitted", "Success", "success")
      this.$router.push({ name: "home-page" });
    },

    legitAddDriver() {
      this.$axios
        .post("/become-driver", {
          id: this.$store.state.currentAccount.id,
          licenseNumber: this.licenseNumber,
          licenseState: this.licenseState
        })
        .then((result) =>{
          this.showSnackbar(result.data.msge);
          if (result.data.ok) {
            this.$store.commit("becomeDriver")
          }
        })
        .catch((err) => this.showSnackbar(err));
    },

    spoofAddDriver() {
      this.showSnackbar("You are now a driver!");
      this.$store.commit("becomeDriver")
      this.$router.push({ name: "home-page" });
    },

    showSnackbar(msge) {
      this.snackbar.msge = msge;
      this.snackbar.show = true;
    },
  },
};
</script>
