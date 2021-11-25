import { initializeApp } from 'firebase/app';
import { getDatabase, update, ref, child, get } from "firebase/database";

const firebaseConfig = {

  apiKey: "AIzaSyCPjBU8uKCuQn-U_yhODASgyLgtcWAfKyU",

  authDomain: "venom-293e9.firebaseapp.com",

  databaseURL: "https://venom-293e9-default-rtdb.firebaseio.com",

  projectId: "venom-293e9",

  storageBucket: "venom-293e9.appspot.com",

  messagingSenderId: "1053305301840",

  appId: "1:1053305301840:web:9289e34601913face0da55"

};


// Initialize Firebase

const app = initializeApp(firebaseConfig);
// Get a reference to the database service
const database = getDatabase(app);

export const query = (op, {data}) => {
    if(op == "update"){
      const { route, states} = data; 
      const updates = {};
      updates['/commands/' + route] = states;
      update(ref(database), updates);
      console.log("Actualizando: "+route);
      console.log(states);
    }else{
      get(child(ref(database), 'commands/D')).then((snapshot) => {
        if (snapshot.exists()) {
          return snapshot.val();
        } else {
          alert("No data available");
        }
      }).catch((error) => {
        alert("error");
      });
    }
    return null;
}
