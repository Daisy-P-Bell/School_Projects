<template>
  <v-container>
    <v-row justify="center">
      <v-col cols="12" sm="8" md="6">
        <v-card class="elevation-12">
          <v-toolbar color="primary" dark flat>
            <v-toolbar-title>Modify Passenger</v-toolbar-title>
          </v-toolbar>
          <v-card-text>
            <v-form>
              <v-text-field
                v-model="name"
                label="Passenger First Name"
                name="firstName"
                type="text"
              />
              <v-text-field
                v-model="name"
                label="Passenger Last Name"
                name="lastName"
                type="text"
              />
              <v-text-field
                v-model="ride"
                label="Ride ID"
                name="ride"
                type="integer"
              />
            </v-form>
          </v-card-text>
          <v-card-actions>
            <v-spacer />
            <v-btn v-on:click="modifyPassenger" color="primary">Modify</v-btn>
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

    modifyPassenger() {
      this.$alert("Passenger Modified", "Success", "success")
      this.$router.push({ name: "admin-page" });
    },

    showSnackbar(msge) {
      this.snackbar.msge = msge;
      this.snackbar.show = true;
    },
  },
};
</script>