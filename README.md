# 21a-emb-aps1

Embarcados - APS 1 - Musical!

Para maiores informações acesse:

https://insper.github.io/ComputacaoEmbarcada/APS-1-Musical/

Issues serão abertos neste repositório para guiar o desenvolvimento
da dupla. **Vocês não devem fechar os issues, apenas a equipe de professores!**, porém devem referenciar nos commits quando um issue 
foi concluído! Isso gerará um PR no classroom que será avaliado pela equipe.

## Documentação

TODO: Aualizar pinos

| Função  | PINO (ex: PA11) |
|---------|-----------------|
| BUZZER  |       PA6       |
| START   |      PD28       |
| SELEÇÃO |      PC31       |

### Imagem da montagem

TODO: Inserir imagem do sistema montado

### Vídeo do projeto

TODO: Inserir link para vídeo do projeto funcionando

Link do vídeo: https://www.dropbox.com/s/527501yqqkrxirt/IMG_3368.MOV?dl=0


### Músicas e Direitos autorais

TODO: Listar músicas e dar crédio da onde saiu:

Referências de músicas: https://github.com/robsoncouto/arduino-songs/blob/master/thegodfather/thegodfather.ino
			https://github.com/robsoncouto/arduino-songs/blob/master/supermariobros/supermariobros.ino
			https://github.com/robsoncouto/arduino-songs/blob/master/tetris/tetris.ino


Código com músicas no arquivo songs.h. Duas structs note e songs foram feitas. A primeira para receber características das notas da música
como frequência e duração e a outra para receber características das músicas, como nome, tempo e suas notas. O vetor das músicas foram organizados
com vetores par nota e duração. 

Utilizamos os botões 1 e 2 da placa OLED para selecionar a música e pausar/play a música, respectivamente. Utilizamos flags e interrupção para isso. 
Há uma indicação visual de qual música está sendo tocada no OLED e o led 1 pisca conforme a música. Todos os periféricos utilizados são definidos
por meio de #defines. 

As músicas escolhidas foram a do Super Mario Bros, The Godfather e Tetris, do repositório indicado. 
