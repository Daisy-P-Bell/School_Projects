<template>
  <v-container>
    <v-row justify="center">
      <v-col cols="12" sm="8" md="6">
        <v-card class="elevation-12">
          <v-toolbar color="primary" dark flat>
            <v-toolbar-title>Add Ride</v-toolbar-title>
          </v-toolbar>
          <v-card-text>
            <v-form>
              <v-text-field
                v-model="date"
                label="Date (e.g. '2020-01-01')"
                name="date"
                type="date"
              />
              <v-text-field
                v-model="time"
                label="Time (e.g. '12:01:01')"
                name="time"
                type="time"
              />
              <v-text-field
                v-model="distance"
                label="Distance"
                name="distance"
                type="distance"
              />
              <v-text-field
                v-model="fuel_price"
                label="Fuel Price"
                name="fuel_price"
                type="fuel_price"
              />
              <v-text-field
                v-model="fee"
                label="Fee"
                name="fee"
                type="fee"
              />
              <v-text-field
                v-model="vehicle"
                label="Vehicle"
                name="vehicle"
                type="vehicle"
              />
              <v-text-field
                v-model="to_location"
                label="To Location"
                name="to_location"
                type="to_location"
              />
              <v-text-field
                v-model="from_location"
                label="From Location"
                name="from_location"
                type="from_location"
              />
            </v-form>
          </v-card-text>
          <v-card-actions>
            <v-spacer />
            <v-btn v-on:click="addRide" color="primary">Submit</v-btn>
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

    addRide() {
      this.$alert("Ride Added", "Success", "success")
      this.$router.push({ name: "admin-page" });
    },

    showSnackbar(msge) {
      this.snackbar.msge = msge;
      this.snackbar.show = true;
    },
  },
};
</script>
