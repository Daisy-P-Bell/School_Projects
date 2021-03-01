<template>
  <v-container>
    <v-row justify="center">
      <v-col cols="12" sm="8" md="6">
        <v-card class="elevation-12">
          <v-toolbar color="primary" dark flat>
            <v-toolbar-title>Add User</v-toolbar-title>
          </v-toolbar>
          <v-card-text>
            <v-form>
              <v-text-field
                v-model="firstName"
                label="User First Name"
                name="firstName"
                type="text"
              />
              <v-text-field
                v-model="lastName"
                label="User Last Name"
                name="lastName"
                type="text"
              />
              <v-text-field
                v-model="email"
                label="Email"
                name="email"
                type="email"
              />
              <v-text-field
                v-model="password"
                label="Password"
                name="password"
                type="password"
              />
              <v-text-field
                v-model="phone"
                label="Phone"
                name="phone"
                type="phone"
              />
              <v-text-field
                v-model="admin"
                label="Is Admin? (e.g. 'True', 'False')"
                name="admin"
                type="text"
              />
            </v-form>
          </v-card-text>
          <v-card-actions>
            <v-spacer />
            <v-btn v-on:click="addUser" color="primary">Submit</v-btn>
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

    addUser() {
      this.$alert("User Added", "Success", "success")
      this.$router.push({ name: "admin-page" });
    },

    showSnackbar(msge) {
      this.snackbar.msge = msge;
      this.snackbar.show = true;
    },
  },
};
</script>
