
let entradaAnalogica, entradaDigital
const tAtualizacao = 500;
const status1 = document.getElementById('status-1')
const status2 = document.getElementById('status-2')
const status3 = document.getElementById('status-3')
// Armazena a url onde roda a aplicação
url = "http://192.168.0.219"

// Função que lê o valor do input e envia para a placa (estrutura: ip/rota?variavel=valor)
function enviaAnalogica() {
  var valor = document.getElementById("saidaAnalogica").value;
  var xhttp = new XMLHttpRequest();
  xhttp.open("GET", url + "/saidaAnalogica?input1=" + valor, true);
  xhttp.send();
}


// A cada 0,5 segundos solicita um valor da placa (estrutura: ip/rota)
// A placa responde com um valor (this.responseText), simulando valor analógico, que é lido pela função interna
function recebeAnalogica() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function () {
    if (this.readyState == 4 && this.status == 200) {
      entradaAnalogica = this.responseText;
      console.log(`EA: ${entradaAnalogica}`);

      if(entradaAnalogica == 'a'){
        status1.style.background = "green"
      }else if(entradaAnalogica == 'A'){
        status1.style.background = "red"

      }
      if(entradaAnalogica == 'b'){
        status2.style.background = "green"

      }else if(entradaAnalogica == 'B'){
        status2.style.background = "red"

      }
      if(entradaAnalogica == 'c'){
        status3.style.background = "green"

      }else if(entradaAnalogica == 'C'){
        status3.style.background = "red"

      }

    }
  };
  xhttp.open("GET", url + "/entradaAnalogica", true);
  xhttp.send();
}
setInterval(recebeAnalogica, tAtualizacao); // chama a função de leitura a cada tAtualizacao milissegundos



setInterval(recebeDigital, tAtualizacao); // chama a função de leitura a cada tAtualizacao milissegundos
