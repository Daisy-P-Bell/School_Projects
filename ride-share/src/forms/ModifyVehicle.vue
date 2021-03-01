<template>
  <v-container>
    <v-row justify="center">
      <v-col cols="12" sm="8" md="6">
        <v-card class="elevation-12">
          <v-toolbar color="primary" dark flat>
            <v-toolbar-title>Modify Vehicle</v-toolbar-title>
          </v-toolbar>
          <v-card-text>
            <v-form>
              <v-text-field
                v-model="make"
                label="Vehicle Make"
                name="make"
                type="text"
              />
              <v-text-field
                v-model="model"
                label="Vehicle Model"
                name="model"
                type="text"
              />
              <v-text-field
                v-model="color"
                label="Vehicle Color"
                name="color"
                type="text"
              />
              <v-text-field
                v-model="type"
                label="Vehcile Type (e.g. 'car', 'SUV', 'van')"
                name="type"
                type="text"
              />
              <v-text-field
                v-model="capacity"
                label="Vehicle Capacity"
                name="capacity"
                type="text"
              />
              <v-text-field
                v-model="mpg"
                label="Vehicle MPG"
                name="mpg"
                type="text"
              />
              <v-text-field
                v-model="license_state"
                label="Vehicle License State"
                name="license_state"
                type="text"
              />
              <v-text-field
                v-model="license_plate"
                label="Vehicle License Plate"
                name="license_plate"
                type="text"
              />
            </v-form>
          </v-card-text>
          <v-card-actions>
            <v-spacer />
            <v-btn v-on:click="modifyVehicle" color="primary">Modify</v-btn>
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

    modifyVehicle() {
      this.$alert("Vehicle Modified", "Success", "success")
      this.$router.push({ name: "admin-page" });
    },

    showSnackbar(msge) {
      this.snackbar.msge = msge;
      this.snackbar.show = true;
    },
  },
};
</script>