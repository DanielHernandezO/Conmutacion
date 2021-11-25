import './App.css';
//import {langs} from './utils/libraries/languages';
import SpeechRecognition, { useSpeechRecognition } from 'react-speech-recognition';
import { query } from './firebase';
const langs =
  [
    ['Español', [['es-AR', 'Argentina'],
    ['es-BO', 'Bolivia'],
    ['es-CL', 'Chile'],
    ['es-CO', 'Colombia'],
    ['es-CR', 'Costa Rica'],
    ['es-EC', 'Ecuador'],
    ['es-SV', 'El Salvador'],
    ['es-ES', 'España'],
    ['es-US', 'Estados Unidos'],
    ['es-GT', 'Guatemala'],
    ['es-HN', 'Honduras'],
    ['es-MX', 'México'],
    ['es-NI', 'Nicaragua'],
    ['es-PA', 'Panamá'],
    ['es-PY', 'Paraguay'],
    ['es-PE', 'Perú'],
    ['es-PR', 'Puerto Rico'],
    ['es-DO', 'República Dominicana'],
    ['es-UY', 'Uruguay'],
    ['es-VE', 'Venezuela']]],

    ['English', [['en-AU', 'Australia'],
    ['en-CA', 'Canada'],
    ['en-IN', 'India'],
    ['en-NZ', 'New Zealand'],
    ['en-ZA', 'South Africa'],
    ['en-GB', 'United Kingdom'],
    ['en-US', 'United States']]]
  ];
import React, { useState, useEffect } from 'react';

const reproducir = (data, dialect) => {
  
  if (!'speechSynthesis' in window) return alert("Lo siento, tu navegador no soporta esta tecnología");

    let textoAEscuchar = "Diagnostico de la casa: Temperatura de "+data.Temperatura+" grados centigrados. Humedad de "+data.Humedad;
    if (!textoAEscuchar) return alert("Escribe el texto");
    let mensaje = new SpeechSynthesisUtterance();
    mensaje.voice = dialect;
    mensaje.volume = 1;
    mensaje.rate = 1;
    mensaje.text = textoAEscuchar;
    mensaje.pitch = 1;
    speechSynthesis.speak(mensaje);
}


const App = () => {
  console.log("Hey", langs);
  const [language, setLanguage] = useState(0);
  const [dialect, setDialect] = useState(langs[0][1][3][0]);
  const [state, setState] = useState(false);
  const [message, setMessage] = useState('');
  let commands = [
    {
      command: ["Pepe"],
      callback: () => {
        setState(true);
        console.log("Detectando funciones...");
      },
      matchInterim: true
    }
  ]
  const all_commands = [
    {
      command: "Abrir spotify y poner *",
      callback: (cancion, spokenPhrase, { resetTranscript }) => {
        const regex = /(reproducir|colocar|poner)/;
        const command = regex.exec(spokenPhrase)[0];
        const position = spokenPhrase.search(command);
        const song = spokenPhrase.substr(position + command.length);
        console.log(song + " command: " + command);

        const data = {
          route: "R",
          states: {
            Cancion: song,
            Estado: 1
          }
        };

        query("update", { data });
        //resetTranscript();
      },
      isFuzzyMatch: true,
      fuzzyMatchingThreshold: 0.2
    },
    {
      command: ['Apagar luces de la casa'],
      callback: () => {
        console.log("Apango luces de la casa");
        const data = {
          route: "l",
          states: {
            Estado: 0
          }
        };

        query("update", { data });
      },
      matchInterim: true
    },
    {
      command: ['Prender ventilador'],
      callback: () => {
        console.log("Modo aire.");
        const data = {
          route: "v",
          states: {
            Estado: 1
          }
        };

        query("update", { data });
      },
      matchInterim: true
    },
    {
      command: ['Apagar ventilador'],
      callback: () => {
        console.log("Modo costa.");
        const data = {
          route: "v",
          states: {
            Estado: 0
          }
        };

        query("update", { data });
      },
      matchInterim: true
    },
    {

      command: 'Cambiar contraseña por *',
      callback: (password, spokenPhrase) => {
        //Matchear con la ReGex
        const regex = /(contraseña a)/;
        const command = regex.exec(spokenPhrase)[0];
        const position = spokenPhrase.search(command);
        const updatePass = spokenPhrase.substr(position + command.length + 1);
        console.log(updatePass + " command: " + command);
        //Actualizacion de clave
        console.log("Contraseña actualizada.");
        const data = {
          route: "Contrasenia",
          states: parseInt(updatePass)
        };

        query("update", { data });
      },
      isFuzzyMatch: true,
      fuzzyMatchingThreshold: 0.2
    },
    {

      command: ['Dar diagnostico de la casa'],
      callback: () => {
        console.log("Generar diagnostico.");
        const data = {
          route: "D",
          states: {
            Estado: 0,
            Fuego: 0,
            Humedad: 0,
            Temperatura: 0
          }
        };

        reproducir(data.states, dialect);
      },
      matchInterim: true
    },
    {
      command: 'Poner luces de la casa en *',
      callback: (cancion, spokenPhrase, { resetTranscript }) => {
        const regex = /(en)/;
        const command = regex.exec(spokenPhrase)[0];
        const position = spokenPhrase.search(command);
        const color = spokenPhrase.substr(position + command.length + 1);
        const states = { "rojo": 1, "azul": 2, "verde": 3 };
        console.log("Luces encendidas. Color: " + color);
        const data = {
          route: "l",
          states: {
            Estado: states[color]
          }
        };

        query("update", { data });
      },
      isFuzzyMatch: true,
      fuzzyMatchingThreshold: 0.2
    },
    {
      command: ['Limpiar'],
      callback: ({ resetTranscript }) => resetTranscript(),
      matchInterim: true
    },
    {
      command: ['Abrir puerta'],
      callback: () => {
        console.log("Abriendo frontera.");
        const data = {
          route: "P",
          states: {
            Estado: 1
          }
        };

        query("update", { data });
      },
      matchInterim: true
    },
    {
      command: ['Cerrar puerta'],
      callback: () => {
        console.log("Cerrando frontera.");
        const data = {
          route: "P",
          states: {
            Estado: 0
          }
        };

        query("update", { data });
      },
      matchInterim: true
    }
  ]


  useEffect(() => {
    setTimeout(() => {
      SpeechRecognition.startListening({ language: dialect, continuous: true });
      console.log("Speaker: " + dialect);
    }, 1500);
  }, [dialect, language]);


  if (state) commands = [...commands, ...all_commands]
  const {
    transcript,
    listening,
    resetTranscript,
    browserSupportsSpeechRecognition
  } = useSpeechRecognition({ commands });

  if (!browserSupportsSpeechRecognition) {
    console.log("Browser doesn't support speech recognition.");
  }

  const handleStartRecording = () => SpeechRecognition.startListening({ language: dialect, continuous: true });
  const handleChangeLanguage = (event) => {
    setLanguage(event.target.selectedIndex);
    setDialect(langs[event.target.selectedIndex][1][0][0]);
    resetTranscript();
    SpeechRecognition.stopListening();
  }
  const handleChangeDialect = (event) => {
    setDialect(event.target.value);
    resetTranscript();
    SpeechRecognition.stopListening();
  };

  return (
    <>
      <div className="mt-4" >
        <p>Microphone: {listening ? 'on' : 'off'}</p>
        <h2 className="mb-3 text-light">Select Language</h2>
        <select className="form-select bg-secondary text-light" onChange={handleChangeLanguage}>
          {

            langs.map((el, key) =>
              <option key={key} value={el[0]}>{el[0]}</option>
            )
          }
        </select>
        <select className="form-select bg-secondary text-light mt-2" onChange={handleChangeDialect}>
          {

            langs[language][1].map((el, key) =>
              <option key={key} value={el[0]}>{el[1]}</option>
            )
          }
        </select>
      </div>
      <h2 className="mt-4 text-light">Transcript</h2>
      <p style={{ color: "white" }}>Dialecto: {dialect}</p>
      <div className="p-3" >
        <p style={{ color: "white" }}>{transcript}</p>
        <p style={{ color: "white" }}>Comando captado: {message}</p>
      </div>
      <div className="mt-4">
        <button className="btn btn-success" onClick={handleStartRecording}>Start</button>
        <button className="btn btn-danger" onClick={SpeechRecognition.stopListening}>Stop</button>
        {listening == "on" && <p className="lead mt-3 text-light" >Listenting ...</p>}
      </div>
    </>
  );

}

export default App;
