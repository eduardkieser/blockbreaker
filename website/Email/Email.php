<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>
        Login 
    </title>
    <link rel="stylesheet" href="Email.css?v=<?=time();?>">
    <link href='https://unpkg.com/boxicons@2.1.4/css/boxicons.min.css' rel='stylesheet'>
</head>
<body
    <div class="b1">
        <a href="../main.php" class="home">Home</a>    
    </div>    
    <div class="login">
        <form action ="Email1.php" method="post">
            <h1>Login</h1>
            <?php if(isset($_GET['error'])){?>
                <p class = "error"><?php echo $_GET['error'] ?> </p>
            <?php }?>
            <div class="EmailId-box">
            <i class='bx bx-envelope'></i>
                <input type="text" name = "EmailID" id = "EmailID" placeholder=" Email ID" required> 
               
            </div>
            <div class="password-box">
                <i class='bx bx-lock-alt' ></i>
                <input type="password" name = "password" id = "password" placeholder=" Password" required> 
            </div>
            <div class="forget">
                <a href="">Forget Password?</a>
            </div>
            <button type="submit" name = "btn" id = "btn" class="btn">Login</button>
            <div class="register">
                <p>Don't have an account? <a href="../Register/Register.html">Register</a></p>
            </div>
        </form>
    </div>    
    
</body>
</html>