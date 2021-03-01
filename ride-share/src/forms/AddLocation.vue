<template>
  <v-container>
    <v-row justify="center">
      <v-col cols="12" sm="8" md="6">
        <v-card class="elevation-12">
          <v-toolbar color="primary" dark flat>
            <v-toolbar-title>Add Location</v-toolbar-title>
          </v-toolbar>
          <v-card-text>
            <v-form>
              <v-text-field
                v-model="name"
                label="Location Name"
                name="name"
                type="text"
              />
              <v-text-field
                v-model="address"
                label="Location Address"
                name="address"
                type="text"
              />
              <v-text-field
                v-model="City"
                label="Location City"
                name="city"
                type="text"
              />
              <v-text-field
                v-model="State"
                label="State (e.g. 'AZ', 'IN', 'NY')"
                name="state"
                type="text"
              />
              <v-text-field
                v-model="Zip"
                label="Zip Code"
                name="zip"
                type="text"
              />
            </v-form>
          </v-card-text>
          <v-card-actions>
            <v-spacer />
            <v-btn v-on:click="addLocation" color="primary">Submit</v-btn>
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

    addLocation() {
      this.$alert("Location Added", "Success", "success")
      this.$router.push({ name: "admin-page" });
    },

    showSnackbar(msge) {
      this.snackbar.msge = msge;
      this.snackbar.show = true;
    },
  },
};
</script>
