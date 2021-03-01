import Vue from "vue";
import Vuetify from "vuetify/lib";
import "vuetify/dist/vuetify.min.css";
import colors from 'vuetify/lib/util/colors'
import VueSimpleAlert from "vue-simple-alert";

Vue.use(Vuetify);
Vue.use(VueSimpleAlert);

export default new Vuetify({
    theme: {
        themes: {
          light: {
            primary: colors.deepOrange,
            secondary: colors.grey.darken1,
            accent: colors.shades.black,
            error: colors.red.accent3,
          },
          dark: {
            primary: colors.blue.lighten3,
          },
        },
      },
});
