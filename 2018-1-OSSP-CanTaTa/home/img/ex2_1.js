var express = require('express');
var app = express();

app.use(express.static('public'));

app.get('/' , function (req,res){
  res.send('hello World! ');
})
app.get('/dynamic',function (req,res){
  var lis = ' ' ;
  for(var i =  0 ; i < 5; i ++)
  {
    lis+='<li>coding</li>';
  }
  var time = Date();

  var output = `
  <!DOCTYPE html>
  <html>
    <head>
      <meta charset='utf-8'>
      <title></title>
    </head>
    <body>
      Hello,
      <ul>
        ${lis}
      </ul>
      ${time}
    </body>
  </html>
  `
  res.send(output);
})
app.get('/route',function(req,res){
  res.send('Hello Router,<img src="/route.jpg">')
})
app.get('/login' , function (req,res){
  res.send('Login please');
})

app.listen(3000,function(){
  console.log('Example app listening on port 3000!');
});
