import Vue from "vue";
import Router from "vue-router";

import Home from "./pages/Home.vue";
import SignIn from "./pages/SignIn.vue";
import SignUp from "./pages/SignUp.vue";
import About from "./pages/About.vue";
import Accounts from "./pages/Accounts.vue";
import FindARide from "./pages/FindARide.vue";
import MyRides from "./pages/MyRides.vue";
import AdminPage from "./pages/Admin.vue";
import BecomeADriver from "./forms/BecomeADriver.vue";
import AddVehicle from "./forms/AddVehicle.vue";
import ModifyVehicle from "./forms/ModifyVehicle.vue";
import AddVehicleType from "./forms/AddVehicleType.vue";
import ModifyVehicleType from "./forms/ModifyVehicleType.vue";
import AddLocation from "./forms/AddLocation.vue";
import ModifyLocation from "./forms/ModifyLocation.vue";
import AddUser from "./forms/AddUser.vue";
import ModifyUser from "./forms/ModifyUser.vue";
import AddRide from "./forms/AddRide.vue";
import ModifyRide from "./forms/ModifyRide.vue";
import AddDriver from "./forms/AddDriver.vue";
import ModifyDriver from "./forms/ModifyDriver.vue";
import AddPassenger from "./forms/AddPassenger.vue";
import ModifyPassenger from "./forms/ModifyPassenger.vue";
import AssignDriverRide from "./forms/AssignDriverRide.vue";
import AssignPassengerRide from "./forms/AssignPassengerRide.vue";
import AssignVehicleRide from "./forms/AssignVehicleRide.vue";
import AuthorizeDriverVehicle from "./forms/AuthorizeDriverVehicle.vue";


Vue.use(Router);

export default new Router({
  mode: "history",
  base: process.env.BASE_URL,
  routes: [
    { name: "home-page", path: "/", component: Home },
    { name: "sign-up", path: "/sign-up", component: SignUp },
    { name: "sign-in", path: "/sign-in", component: SignIn },
    { name: "about-us", path: "/about-us", component: About },
    { name: "accounts", path: "/accounts", component: Accounts },
    { name: "find-a-ride", path: "/find-a-ride", component: FindARide },
    { name: "become-a-driver", path: "/become-a-driver", component: BecomeADriver },
    { name: "my-rides", path: "/my-rides", component: MyRides },
    { name: "admin-page", path: "/admin", component: AdminPage },

    { name: "add-vehicle", path: "/add-vehicle", component: AddVehicle },
    { name: "modify-vehicle", path: "/modify-vehicle", component: ModifyVehicle },
    { name: "add-vehicle-type", path: "/add-vehicle-type", component: AddVehicleType },
    { name: "modify-vehicle-type", path: "/modify-vehicle-type", component: ModifyVehicleType },
    { name: "add-location", path: "/add-location", component: AddLocation },
    { name: "modify-location", path: "/modify-location", component: ModifyLocation },
    { name: "add-user", path: "/add-user", component: AddUser },
    { name: "modify-user", path: "/modify-user", component: ModifyUser },
    { name: "add-ride", path: "/add-ride", component: AddRide },
    { name: "modify-ride", path: "/modify-ride", component: ModifyRide },
    { name: "add-driver", path: "/add-driver", component: AddDriver },
    { name: "modify-driver", path: "/modify-driver", component: ModifyDriver },
    { name: "add-passenger", path: "/add-passenger", component: AddPassenger },
    { name: "modify-passenger", path: "/modify-passenger", component: ModifyPassenger },

    { name: "assign-driver-ride", path: "/assign-driver-ride", component: AssignDriverRide },
    { name: "assign-passenger-ride", path: "/assign-passenger-ride", component: AssignPassengerRide },
    { name: "assign-vehicle-ride", path: "/assign-vehicle-ride", component: AssignVehicleRide },
    { name: "authorize-driver-vehicle", path: "/authorize-driver-vehicle", component: AuthorizeDriverVehicle },

  ]
});
