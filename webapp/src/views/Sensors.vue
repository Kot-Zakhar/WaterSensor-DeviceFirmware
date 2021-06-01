<template>
  <div>
    <v-container>
      <v-card>
        <v-card-title class="text-h5">Sensors boundaries</v-card-title>
        <v-divider></v-divider>
        <v-card-title class="text-h6">Water sensor</v-card-title>
        <v-card-text>
          <v-range-slider
            v-model="waterSensor.range"
            :max="waterSensor.max"
            :min="waterSensor.min"
            class="align-center"
          >
            <template v-slot:prepend>
              <v-text-field
                :value="waterSensor.range[0]"
                class="mt-0 pt-0"
                hide-details
                single-line
                type="number"
                style="width: 60px"
                @change="$set(waterSensor.range, 0, $event)"
              ></v-text-field>
            </template>
            <template v-slot:append>
              <v-text-field
                :value="waterSensor.range[1]"
                class="mt-0 pt-0"
                hide-details
                single-line
                type="number"
                style="width: 60px"
                @change="$set(waterSensor.range, 1, $event)"
              ></v-text-field>
            </template>
          </v-range-slider>
        </v-card-text>
        <v-divider></v-divider>
        <v-card-title class="text-h6">Humidity sensor</v-card-title>
        <v-card-text>
          <v-range-slider
            v-model="humidSensor.range"
            :max="humidSensor.max"
            :min="humidSensor.min"
            class="align-center"
          >
            <template v-slot:prepend>
              <v-text-field
                :value="humidSensor.range[0]"
                class="mt-0 pt-0"
                hide-details
                single-line
                type="number"
                style="width: 60px"
                @change="$set(humidSensor.range, 0, $event)"
              ></v-text-field>
            </template>
            <template v-slot:append>
              <v-text-field
                :value="humidSensor.range[1]"
                class="mt-0 pt-0"
                hide-details
                single-line
                type="number"
                style="width: 60px"
                @change="$set(humidSensor.range, 1, $event)"
              ></v-text-field>
            </template>
          </v-range-slider>
        </v-card-text>
        <v-divider></v-divider>
        <v-card-title class="text-h6">Temperature sensor</v-card-title>
        <v-card-text>
          <v-range-slider
            v-model="tempSensor.range"
            :max="tempSensor.max"
            :min="tempSensor.min"
            class="align-center"
          >
            <template v-slot:prepend>
              <v-text-field
                :value="tempSensor.range[0]"
                class="mt-0 pt-0"
                hide-details
                single-line
                type="number"
                style="width: 60px"
                @change="$set(tempSensor.range, 0, $event)"
              ></v-text-field>
            </template>
            <template v-slot:append>
              <v-text-field
                :value="tempSensor.range[1]"
                class="mt-0 pt-0"
                hide-details
                single-line
                type="number"
                style="width: 60px"
                @change="$set(tempSensor.range, 1, $event)"
              ></v-text-field>
            </template>
          </v-range-slider>
        </v-card-text>
        <v-divider></v-divider>
        <v-card-actions>
          <v-btn
            color="primary"
            class="mx-auto"
            @click="saveBoundaries"
          >
            <v-icon left>mdi-content-save</v-icon>
            Save
          </v-btn>
        </v-card-actions>
      </v-card>
    </v-container>

    <v-container>
      <v-card>
        <v-card-title class="text-h5">Live sensor check</v-card-title>
        <v-divider></v-divider>
        <v-card-title class="text-h6">Water sensor</v-card-title>
        <v-card-text>
          <apexchart ref="waterChart" type="line" height="350" :options="chartOptions" :series="waterSeries"></apexchart>
          <v-btn
            @click="addValue">add</v-btn>
        </v-card-text>
      </v-card>
    </v-container>
  </div>
</template>

<script>
// import SensorChart from '../components/SensorChart'
import VueApexCharts from 'vue-apexcharts'

export default {
  name: 'Sensors',
  components: {
    apexchart: VueApexCharts
  },
  data() {
    return {
      waterSensor: {
        range: [1000, 2000],
        min: 0,
        max: 2048
      },
      waterSeries: [
        {
          name: "Min bound",
          data: []
        },
        {
          name: 'Max bound',
          data: [],
        },
        {
          name: 'Sensor value',
          data: [],
        }
      ],
      chartOptions: {
        chart: {
          height: 350,
          type: 'area',
          zoom: {
            enabled: false
          }
        },
        colors: ["#F00", "#F00", "#000"],
        animations: {
          enabled: true,
          easing: 'linear',
          dynamicAnimation: {
            speed: 1000
          }
        },
        dataLabels: {
          enabled: false
        },
        stroke: {
          curve: 'smooth'
        },
        grid: {
          row: {
            colors: ['#f3f3f3', 'transparent'], // takes an array which will be repeated on columns
            opacity: 0.5
          }
        },
        xaxis: {
          type: 'datetime'
        }
      },
      tempSensor: {
        range: [20, 40],
        min: 0,
        max: 50
      },
      humidSensor: {
        range: [30, 50],
        min: 20,
        max: 80
      },
      checkTimer: null
    }
  },
  created() {
    this.checkTimer = setInterval(() => {
      this.addValue();
    }, 1000)
  },
  beforeDestroy() {
    clearInterval(this.checkTimer);
  },
  methods: {
    async saveBoundaries() {

    },
    async fetchWebSocketAddress() {

    },
    addValue() {
      this.$refs.waterChart.appendData([{
        data: [[Date.now(), this.waterSensor.range[0]]]
      },{
        data: [[Date.now(), this.waterSensor.range[1]]]
      }, {
        data: [[Date.now(), Math.floor(Math.random() * (100) + 1000)]]
      }]);
    }
  }
}
</script>

<style>

</style>