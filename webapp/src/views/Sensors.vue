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
            <v-icon left>{{ mdiContentSave }}</v-icon>
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
          <apexchart ref="waterSensorChart" type="line" height="350" :options="chartOptions" :series="waterSeries"></apexchart>
        </v-card-text>
        <v-divider></v-divider>
        <v-card-title class="text-h6">Humidity sensor</v-card-title>
        <v-card-text>
          <apexchart ref="humidSensorChart" type="line" height="350" :options="chartOptions" :series="humidSeries"></apexchart>
        </v-card-text>
        <v-divider></v-divider>
        <v-card-title class="text-h6">Temperature sensor</v-card-title>
        <v-card-text>
          <apexchart ref="tempSensorChart" type="line" height="350" :options="chartOptions" :series="tempSeries"></apexchart>
        </v-card-text>
      </v-card>
    </v-container>
  </div>
</template>

<script>
import VueApexCharts from 'vue-apexcharts'
import { mdiContentSave } from '@mdi/js'

export default {
  name: 'Sensors',
  components: {
    apexchart: VueApexCharts
  },
  data() {
    return {
      mdiContentSave,
      waterSensor: {
        range: [1000, 2000],
        min: 0,
        max: 2000
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
      tempSensor: {
        range: [10, 40],
        min: 0,
        max: 50
      },
      tempSeries: [
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
      humidSensor: {
        range: [20, 50],
        min: 20,
        max: 80
      },
      humidSeries: [
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
      checkTimer: null
    }
  },
  created() {
    this.checkTimer = setInterval(() => {
      this.fetchValuesAndUpdateChart();
    }, 1000),
    this.getBoundaries();
  },
  beforeDestroy() {
    clearInterval(this.checkTimer);
  },
  methods: {
    async getBoundaries() {
      const response = await fetch(this.$api + "/api/sensors-boundaries", {method: "GET"});
      const data = await response.json();

      if (data.status == "OK" && data.payload) {
        console.log(data);
        this.waterSensor.range = [data.payload.waterSensor.low, data.payload.waterSensor.high];
        this.tempSensor.range = [data.payload.tempSensor.low, data.payload.tempSensor.high];
        this.humidSensor.range = [data.payload.humidSensor.low, data.payload.humidSensor.high];
      }
    },
    async saveBoundaries() {
      const reqOptions = {
        method: "POST",
        headers: {
          "Content-Type": "application/json"
        },
        body: JSON.stringify({
          waterSensor: {
            low: this.waterSensor.range[0],
            high: this.waterSensor.range[1]
          },
          tempSensor: {
            low: this.tempSensor.range[0],
            high: this.tempSensor.range[1]
          },
          humidSensor: {
            low: this.humidSensor.range[0],
            high: this.humidSensor.range[1]
          },
        })
      };
      const res = await fetch(this.$api + "/api/sensors-boundaries", reqOptions);
      const data = await res.json();
      if (data.status !== "OK") {
        console.error(data);
      }
    },
    async fetchValuesAndUpdateChart() {
      const response = await fetch(this.$api + "/api/sensors-values", {method: "GET"});
      const data = await response.json();

      if (data.status == "OK") {
        this.addValue("waterSensor", data.payload.waterSensor);
        this.addValue("tempSensor", data.payload.tempSensor);
        this.addValue("humidSensor", data.payload.humidSensor);
      }
    },
    addValue(name, value) {
      const time = Date.now();
      this.$refs[name + "Chart"].appendData([{
        data: [[time, this[name].range[0]]]
      },{
        data: [[time, this[name].range[1]]]
      }, {
        data: [[time, value]]
      }]);
    }
  }
}
</script>

<style>

</style>