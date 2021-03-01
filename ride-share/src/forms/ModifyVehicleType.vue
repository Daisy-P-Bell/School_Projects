<template>
  <v-container>
    <v-row justify="center">
      <v-col cols="12" sm="8" md="6">
        <v-card class="elevation-12">
          <v-toolbar color="primary" dark flat>
            <v-toolbar-title>Add Vehicle Type</v-toolbar-title>
          </v-toolbar>
          <v-card-text>
            <v-form>
              <v-text-field
                v-model="vehicle_type_id"
                label="Vehcile Type ID"
                name="vehicle_type_id"
                type="integer"
              />
              <v-text-field
                v-model="type"
                label="Vehcile Type (e.g. 'car', 'SUV', 'van')"
                name="type"
                type="text"
              />
            </v-form>
          </v-card-text>
          <v-card-actions>
            <v-spacer />
            <v-btn v-on:click="modifyVehicleType" color="primary">Modify</v-btn>
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

    modifyVehicleType() {
      this.$alert("Vehicle Type Modified", "Success", "success")
      this.$router.push({ name: "admin-page" });
    },

    showSnackbar(msge) {
      this.snackbar.msge = msge;
      this.snackbar.show = true;
    },
  },
};
</script>