<template>
  <v-container>
    <div>
      <h4 class="display-2">Admin</h4><br><br>

      <!-- Admin Actions -->
      <h6 class="display-1">Admin Actions  <br>          
        <v-btn color="primary" text v-bind:to="{ name: 'assign-vehicle-ride' }">Assign Vehicle to Ride</v-btn><br>
        <v-btn color="primary" text v-bind:to="{ name: 'authorize-driver-vehicle' }">Authorize Driver for Vehicle</v-btn><br>
        <v-btn color="primary" text v-bind:to="{ name: 'assign-driver-ride' }">Assign Driver to Ride</v-btn><br>
        <v-btn color="primary" text v-bind:to="{ name: 'assign-passenger-ride' }">Assign Passenger to Ride</v-btn>
      </h6>
      <br> <br>


      <!-- Rides Table -->
      <h6 class="display-1">Rides            <v-btn color="primary" text v-bind:to="{ name: 'add-ride' }">
        Add a Ride
      </v-btn></h6>

      <v-text-field
        v-model="search"
        append-icon="mdi-magnify"
        label="Search"
        single-line
        hide-details
      ></v-text-field>

      <v-data-table
        class="elevation-1"
        v-bind:headers="ride_headers"
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
            <td>{{ item.vehicle_color + ", " + item.vehicle_make + " " + item.vehicle_model}}</td>
            <td>{{ item.vehicle_capacity }}</td>
            <td>
              <v-icon small class="ml-2" @click="deleteRide(item)">
                mdi-delete
              </v-icon>
              <v-icon small class="ml-2" @click="modifyRide()">
                mdi-pencil
              </v-icon>
            </td>
          </tr>
        </template>
      </v-data-table> <br> <br>

      <!-- Vehicle Table -->
      <h6 class="display-1">Vehicles            <v-btn color="primary" text v-bind:to="{ name: 'add-vehicle' }">
        Add a Vehicle
      </v-btn></h6>

      <v-text-field
        v-model="search"
        append-icon="mdi-magnify"
        label="Search"
        single-line
        hide-details
      ></v-text-field>

      <v-data-table
        class="elevation-1"
        v-bind:headers="vehicle_headers"
        v-bind:items="vehicles"
        v-bind:search="search"
      >
        <template v-slot:item="{ item }">
          <tr v-bind:class="itemClass(item)">
            <td>{{ item.make }}</td>
            <td>{{ item.model }}</td>
            <td>{{ item.color }}</td>
            <td>{{ item.capacity }}</td>
            <td>{{ item.mpg }}</td>
            <td>{{ item.licence_state }}</td>
            <td>{{ item.licence_plate }}</td>
            <td>
              <v-icon small class="ml-2" @click="deleteVehicle(item)">
                mdi-delete
              </v-icon>
              <v-icon small class="ml-2" @click="modifyVehicle()">
                mdi-pencil
              </v-icon>
            </td>
          </tr>
        </template>
      </v-data-table><br> <br>

      <!-- Vehicle Type Table -->
      <h6 class="display-1">Vehicle Types            <v-btn color="primary" text v-bind:to="{ name: 'add-vehicle-type' }">
        Add a Vehicle Type
      </v-btn></h6>

      <v-text-field
        v-model="search"
        append-icon="mdi-magnify"
        label="Search"
        single-line
        hide-details
      ></v-text-field>

      <v-data-table
        class="elevation-1"
        v-bind:headers="vehicle_type_headers"
        v-bind:items="vehicle_types"
        v-bind:search="search"
      >
        <template v-slot:item="{ item }">
          <tr v-bind:class="itemClass(item)">
            <td>{{ item.type }}</td>
            <td>
              <v-icon small class="ml-2" @click="deleteVehicleType(item)">
                mdi-delete
              </v-icon>
              <v-icon small class="ml-2" @click="modifyVehicleType()">
                mdi-pencil
              </v-icon>
            </td>
          </tr>
        </template>
      </v-data-table>

      <br> <br>
      <!-- Locations Table -->
      <h6 class="display-1">Locations            <v-btn color="primary" text v-bind:to="{ name: 'add-location' }">
        Add a Location
      </v-btn></h6>

      <v-text-field
        v-model="search"
        append-icon="mdi-magnify"
        label="Search"
        single-line
        hide-details
      ></v-text-field>

      <v-data-table
        class="elevation-1"
        v-bind:headers="location_headers"
        v-bind:items="locations"
        v-bind:search="search"
      >
        <template v-slot:item="{ item }">
          <tr v-bind:class="itemClass(item)">
            <td>{{ item.name }}</td>
            <td>{{ item.address }}</td>
            <td>{{ item.city }}</td>
            <td>{{ item.state }}</td>
            <td>{{ item.zip_code }}</td>
            <td>
              <v-icon small class="ml-2" @click="deleteLocation(item)">
                mdi-delete
              </v-icon>
              <v-icon small class="ml-2" @click="modifyLocation()">
                mdi-pencil
              </v-icon>
            </td>
          </tr>
        </template>
      </v-data-table>

      <br> <br>
      <!-- Users Table -->
      <h6 class="display-1">Users            <v-btn color="primary" text v-bind:to="{ name: 'add-user' }">
        Add a User
      </v-btn></h6>

      <v-text-field
        v-model="search"
        append-icon="mdi-magnify"
        label="Search"
        single-line
        hide-details
      ></v-text-field>

      <v-data-table
        class="elevation-1"
        v-bind:headers="user_headers"
        v-bind:items="users"
        v-bind:search="search"
      >
        <template v-slot:item="{ item }">
          <tr v-bind:class="itemClass(item)">
            <td>{{ item.firstName }}</td>
            <td>{{ item.lastName }}</td>
            <td>{{ item.email }}</td>
            <td>{{ item.phone }}</td>
            <td>{{ item.isAdmin }}</td>
            <td>
              <v-icon small class="ml-2" @click="deleteUser(item)">
                mdi-delete
              </v-icon>
              <v-icon small class="ml-2" @click="modifyUser()">
                mdi-pencil
              </v-icon>
            </td>
          </tr>
        </template>
      </v-data-table>

      <br> <br>
      <!-- Drivers Table -->
      <h6 class="display-1">Drivers            <v-btn color="primary" text v-bind:to="{ name: 'add-driver' }">
        Add a Driver
      </v-btn></h6>

      <v-text-field
        v-model="search"
        append-icon="mdi-magnify"
        label="Search"
        single-line
        hide-details
      ></v-text-field>

      <v-data-table
        class="elevation-1"
        v-bind:headers="driver_headers"
        v-bind:items="drivers"
        v-bind:search="search"
      >
        <template v-slot:item="{ item }">
          <tr v-bind:class="itemClass(item)">
            <td>{{ item.firstName }}</td>
            <td>{{ item.lastName }}</td>
            <td>{{ item.phone }}</td>
            <td>{{ item.licenseNumber }}</td>
            <td>{{ item.licenseState }}</td>
            <td>
              <v-icon small class="ml-2" @click="deleteDriver(item)">
                mdi-delete
              </v-icon>
              <v-icon small class="ml-2" @click="modifyDriver()">
                mdi-pencil
              </v-icon>
            </td>
          </tr>
        </template>
      </v-data-table>

      <br> <br>
      <!-- Passengers Table -->
      <h6 class="display-1">Passengers            <v-btn color="primary" text v-bind:to="{ name: 'add-passenger' }">
        Add a Passenger
      </v-btn></h6>

      <v-text-field
        v-model="search"
        append-icon="mdi-magnify"
        label="Search"
        single-line
        hide-details
      ></v-text-field>

      <v-data-table
        class="elevation-1"
        v-bind:headers="passenger_headers"
        v-bind:items="passengers"
        v-bind:search="search"
      >
        <template v-slot:item="{ item }">
          <tr v-bind:class="itemClass(item)">
            <td>{{ item.firstName }}</td>
            <td>{{ item.lastName }}</td>
            <td>{{ item.date }}</td>
            <td>{{ item.time }}</td>
            <td>{{ item.distance }}</td>
            <td>
              <v-icon small class="ml-2" @click="deletePassenger(item)">
                mdi-delete
              </v-icon>
              <v-icon small class="ml-2" @click="modifyPassenger()">
                mdi-pencil
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
  ride_name: "Rides",
  vehicle_name: "Vehicles",

  data: function() {
    return {
      search: "",
      ride_headers: [
        { text: "Date", value: "date" },
        { text: "Time", value: "time" },
        { text: "To Location", value: "to_name" },
        { text: "From Location", value: "from_name" },
        { text: "Address", value: "address" },
        { text: "City", value: "city" },
        { text: "State", value: "state" },
        { text: "ZIP", value: "zip_code" },
        { text: "Vehicle", value: "vehicle_make" },
        { text: "Capacity", value: "vehicle_capacity" },
        { text: "Action", value: "action" },
      ],
      vehicle_headers: [
        { text: "Make", value: "make" },
        { text: "Model", value: "model" },
        { text: "Color", value: "color" },
        { text: "Capacity", value: "capacity" },
        { text: "MPG", value: "mpg" },
        { text: "License State", value: "licence_state" },
        { text: "License Plate", value: "licence_plate" },
        { text: "Action", value: "action" },
      ],
      vehicle_type_headers: [
        { text: "Type", value: "type" },
        { text: "Action", value: "action" },
      ],
      location_headers: [
        { text: "Name", value: "name" },
        { text: "Address", value: "address" },
        { text: "City", value: "city" },
        { text: "State", value: "state" },
        { text: "ZIP", value: "zip_code" },
        { text: "Action", value: "action" },
      ],
      user_headers: [
        { text: "First Name", value: "firstName" },
        { text: "Last Name", value: "lastName" },
        { text: "Email", value: "email" },
        { text: "Phone", value: "phone" },
        { text: "Admin?", value: "isAdmin" },
        { text: "Action", value: "action" },
      ],
      driver_headers: [
        { text: "First Name", value: "firstName" },
        { text: "Last Name", value: "lastName" },
        { text: "Phone", value: "phone" },
        { text: "License Number", value: "licenseNumber" },
        { text: "License State", value: "licenseState" },
        { text: "Action", value: "action" },
      ],
      passenger_headers: [
        { text: "First Name", value: "firstName" },
        { text: "Last Name", value: "lastName" },
        { text: "Date", value: "date" },
        { text: "Time", value: "time" },
        { text: "Distance", value: "distance" },
        { text: "Action", value: "action" },
      ],
      rides: [],
      vehicles: [],
      vehicle_types: [],
      locations: [],
      users: [],
      drivers: [],
      passengers: [],

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
          vehicle_make: ride.vehicles.make,
          vehicle_model: ride.vehicles.model,
          vehicle_color: ride.vehicles.color,
          vehicle_capacity: ride.vehicles.capacity,
        })
      );
    }),

    this.$axios.get("/vehicles").then((response) => {
      console.log("RESPONSE", response);
      this.vehicles = response.data.map((vehicle) =>
        //console.log(ride.to_locations.zip_code),
        (
          console.log("VEHICLE", vehicle),
          {
          vehicle_id: vehicle.vehicle_id,
          make: vehicle.make,
          model: vehicle.model,
          color: vehicle.color,
          capacity: vehicle.capacity,
          mpg: vehicle.mpg,
          licence_state: vehicle.licence_state,
          licence_plate: vehicle.licence_plate,
        })
      );
    }),

    this.$axios.get("/vehicle_types").then((response) => {
      console.log("RESPONSE", response);
      this.vehicle_types = response.data.map((vehicle_type) =>
        //console.log(ride.to_locations.zip_code),
        (
          console.log("VEHICLE TYPE", vehicle_type),
          {
          vehicle_type_id: vehicle_type.vehicle_type_id,
          type: vehicle_type.type,
        })
      );
    }),

    this.$axios.get("/locations").then((response) => {
      console.log("RESPONSE", response);
      this.locations = response.data.map((location) =>
        //console.log(ride.to_locations.zip_code),
        (
          console.log("LOCATION", location),
          {
          location_id: location.location_id,
          name: location.name,
          address: location.address,
          city: location.city,
          state: location.state,
          zip_code: location.zip_code,
        })
      );
    }),

    this.$axios.get("/users").then((response) => {
      console.log("RESPONSE", response);
      this.users = response.data.map((user) =>
        //console.log(ride.to_locations.zip_code),
        (
          console.log("USER", user),
          {
          user_id: user.user_id,
          firstName: user.firstName,
          lastName: user.lastName,
          email: user.email,
          phone: user.phone,
          isAdmin: user.isAdmin,
        })
      );
    }),

    this.$axios.get("/drivers").then((response) => {
      console.log("RESPONSE", response);
      this.drivers = response.data.map((driver) =>
        //console.log(ride.to_locations.zip_code),
        (
          console.log("DRIVER", driver),
          {
          driver_id: driver.driver_id,
          user_id: driver.users.user_id,
          firstName: driver.users.firstName,
          lastName: driver.users.lastName,
          phone: driver.users.phone,
          licenseNumber: driver.licenseNumber,
          licenseState: driver.licenseState,
        })
      );
    }),

    this.$axios.get("/passengers").then((response) => {
      console.log("RESPONSE", response);
      this.passengers = response.data.map((passenger) =>
        //console.log(ride.to_locations.zip_code),
        (
          console.log("PASSENGER", passenger),
          {
          passenger_id: passenger.passenger_id,
          ride_id: passenger.ride_id,
          firstName: passenger.users.firstName,
          lastName: passenger.users.lastName,
          date: passenger.rides.date,
          time: passenger.rides.time,
          distance: passenger.rides.distance,
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

    // RIDE FUNCTIONS
      // Remove a ride listing
      deleteRide(item) {
        console.log("UPDATE", JSON.stringify(item, null, 2));
        console.log("CURRENT ride ID: ", item.ride_id);
        this.$axios.delete(`/ride/${item.ride_id}`);
        this.rides = this.rides.filter((ride) => ride.ride_id !== item.ride_id);
        this.showSnackbar("Ride removed.");
      },

      // Modify ride information.
      modifyRide() {
        this.$router.push({ name: 'modify-ride' })
      },

    // VEHICLE FUNCTIONS
      // Remove a vehicle listing.
      deleteVehicle(item) {
        console.log("UPDATE", JSON.stringify(item, null, 2));
        console.log("CURRENT vehicle ID: ", item.vehicle_id);
        this.$axios.delete(`/vehicle/${item.vehicle_id}`);
        this.vehicles = this.vehicles.filter((vehicle) => vehicle.vehicle_id !== item.vehicle_id);
        this.showSnackbar("Vehicle removed.");
      },

      // Modify vehicle information.
      modifyVehicle() {
        this.$router.push({ name: 'modify-vehicle' })
      },


    // VEHICLE TYPE FUNCTIONS
      // Remove a vehicle type listing.
      deleteVehicleType(item) {
        console.log("UPDATE", JSON.stringify(item, null, 2));
        console.log("CURRENT vehicle type ID: ", item.vehicle_type_id);
        this.$axios.delete(`/vehicle_type/${item.vehicle_type_id}`);
        this.vehicle_types = this.vehicle_types.filter((vehicle_type) => vehicle_type.vehicle_type_id !== item.vehicle_type_id);
        this.showSnackbar("Vehicle Type removed.");
      },

      // Modify vehicle type information.
      modifyVehicleType() {
        this.$router.push({ name: 'modify-vehicle-type' })
      },

    // LOCATION FUNCTIONS
      // Remove a location listing.
      deleteLocation(item) {
        console.log("UPDATE", JSON.stringify(item, null, 2));
        console.log("CURRENT location ID: ", item.location_id);
        this.$axios.delete(`/location/${item.location_id}`);
        this.locations = this.locations.filter((location) => location.location_id !== item.location_id);
        this.showSnackbar("Location removed.");
      },

      // Modify location information.
      modifyLocation() {
        this.$router.push({ name: 'modify-location' })
      },

    // USER FUNCTIONS
      // Remove a user listing.
      deleteUser(item) {
        console.log("UPDATE", JSON.stringify(item, null, 2));
        console.log("CURRENT user ID: ", item.user_id);
        this.$axios.delete(`/user/${item.user_id}`);
        this.users = this.users.filter((user) => user.user_id !== item.user_id);
        this.showSnackbar("User removed.");
      },

      // Modify user information.
      modifyUser() {
        this.$router.push({ name: 'modify-user' })
      },

    // DRIVER FUNCTIONS
      // Remove a driver listing.
      deleteDriver(item) {
        console.log("UPDATE", JSON.stringify(item, null, 2));
        console.log("CURRENT driver ID: ", item.driver_id);
        this.$axios.delete(`/driver/${item.driver_id}`);
        this.drivers = this.drivers.filter((driver) => driver.driver_id !== item.driver_id);
        this.showSnackbar("Driver removed.");
      },

      // Modify driver information.
      modifyDriver() {
        this.$router.push({ name: 'modify-driver' })
      },

    // PASSENGER FUNCTIONS
      // Remove a passenger listing.
      deletePassenger(item) {
        console.log("UPDATE", JSON.stringify(item, null, 2));
        console.log("CURRENT passenger ID: ", item.passenger_id);
        //this.$axios.delete(`/passenger/${item.driver_id}`);
        this.$axios.post("/passenger", {
          passenger_id: item.passenger_id,
          ride_id: item.ride_id,
        });
        this.passengers = this.passengers.filter((passenger) => ((passenger.passenger_id !== item.passenger_id) && (passenger.ride_id !== item.ride_id)));
        this.showSnackbar("Passenger removed.");
      },

      // Modify passenger information.
      modifyPassenger() {
        this.$router.push({ name: 'modify-passenger' })
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
