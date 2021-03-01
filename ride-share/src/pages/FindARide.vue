<template>
  <v-container>
    <div>
      <h4 class="display-1">Find a Ride</h4>

      <v-text-field
        v-model="search"
        append-icon="mdi-magnify"
        label="Search"
        single-line
        hide-details
      ></v-text-field>

      <v-data-table
        class="elevation-1"
        v-bind:headers="headers"
        v-bind:items="rides"
        v-bind:search="search"
      >
        <template v-slot:item="{ item }">
          <tr v-bind:class="itemClass(item)">
            <td>{{ item.date }}</td>
            <td>{{ item.time }}</td>
            <td>{{ item.to_name }}</td>
            <td>{{ item.from_name }}</td>
            <td>{{ item.address }}</td>
            <td>{{ item.city }}</td>
            <td>{{ item.state }}</td>
            <td>{{ item.zip_code }}</td>
            <td>{{ item.vehicle_capacity }}</td>
            <td>
              <v-icon small class="ml-2" @click="addRide(item)">
                mdi-transit-transfer
              </v-icon>
            </td>
          </tr>
        </template>
      </v-data-table>

      <v-snackbar v-model="snackbar.show">
        {{ snackbar.text }}
        <v-btn color="blue" text @click="snackbar.show = false">
          Close
        </v-btn>
      </v-snackbar>
    </div>
  </v-container>
</template>

<script>
export default {
  name: "Rides",

  data: function() {
    return {
      search: "",
      headers: [
        { text: "Date", value: "date" },
        { text: "Time", value: "time" },
        { text: "To Location", value: "to_name" },
        { text: "From Location", value: "from_name" },
        { text: "Address", value: "address" },
        { text: "City", value: "city" },
        { text: "State", value: "state" },
        { text: "ZIP", value: "zip_code" },
        { text: "Capacity", value: "vehicle_capacity" },
        { text: "Action", value: "action" },
      ],
      rides: [],

      snackbar: {
        show: false,
        text: "",
      },
    };
  },

  mounted: function() {
    this.$axios.get("/rides").then((response) => {
      console.log("RESPONSE", response);
      this.rides = response.data.map((ride) =>
        //console.log(ride.to_locations.zip_code),
        (
          console.log("RIDE", ride),
          {
          ride_id: ride.ride_id,
          date: ride.date,
          time: ride.time,
          to_name: ride.to_locations.name,
          from_name: ride.from_locations.name,
          address: ride.to_locations.address,
          city: ride.to_locations.city,
          state: ride.to_locations.state,
          zip_code: ride.to_locations.zip_code,
          vehicle_capacity: ride.vehicles.capacity,
        })
      );
    });
  },

  methods: {
    // Display a snackbar message.
    showSnackbar(text) {
      this.snackbar.text = text;
      this.snackbar.show = true;
    },

    // Calculate the CSS class for an item
    itemClass(item) {
      const currentAccount = this.$store.state.currentAccount;
      if (currentAccount && currentAccount.id === item.id) {
        return "currentAccount";
      }
      return currentAccount;
    },

    // Sign up for a ride
    addRide(item) {
      console.log("RIDE", item);
      this.$axios.post("/rides", {
        passenger_id: this.$store.state.currentAccount.id,
        ride_id: item.ride_id,
      });
      console.log("UPDATE", JSON.stringify(item, null, 2));
      this.showSnackbar("Signed up for ride!");
    },

    // Update account information.
    updateAccount(item) {
      console.log("UPDATE", JSON.stringify(item, null, 2));
      this.showSnackbar("Sorry, update is not yet implemented.");
    },

    // Delete an account.
    deleteAccount(item) {
      this.$axios.delete(`/accounts/${item.id}`).then((response) => {
        if (response.data.ok) {
          // The delete operation worked on the server; delete the local account
          // by filtering the deleted account from the list of accounts.
          this.rides = this.rides.filter((ride) => ride.id !== item.id);
        }
      });
    },
  },
};
</script>

<style>
.currentAccount {
  background: lightcoral;
}
</style>
