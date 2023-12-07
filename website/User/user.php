<?php
    session_start();
    $output = "";
    $conn = mysqli_connect("localhost","root","","block_breaker");
    $username = $_SESSION['UserName'];
    $sel = "SELECT * FROM user_image WHERE username = '$username'";
    $que = mysqli_query($conn, $sel);
    $counter = 0;
?>
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>
        Block Breaker
    </title>
    <link rel="stylesheet" href="user.css?v=<?=time();?>">
</head>
<body>
    <div class = "username">
            <a href="../logout.php" class="logout">
                <button class="logout1">
                    Logout
                </button>
            </a>
        <h9 class = "username-box"> <?=$_SESSION['UserName'];?></h9>
        <a href="../main.php" class="home">
                Home
            </a>
    </div>
    <div class = "img">
            <?php 
            if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_POST['main_id'])){
                if($que){
                    mysqli_data_seek($que, $_POST['main_id'] - 1);
                    $row1 = mysqli_fetch_array($que);
                    if($row1){
                        $_SESSION['FileName'] = $row1['Name'];
                        header("location:./User_data/User_data.php");
                    }
                }
            }
            while ($row = mysqli_fetch_array($que)) {
                $counter++;
                $MainId = $counter;
                $HeadId = 'Name' . $counter;
                $mId = 'M'.$counter;
                ?>
                <div id = "<?php echo $mId;?>" class="img1">
                    <?php if ($row["Name"] == null) : ?>
                        <script>
                            function submitForm() {
                                document.getElementById("uploadForm").submit();
                            }
                            function submitText() {
                                // Get the text from the text input and perform an action
                                var textValue = document.getElementById('text').value;
                                hideForm1();
                                fetch('text.php', {
                                method: 'POST',
                                headers: {
                                    'Content-Type': 'application/x-www-form-urlencoded',
                                },
                                body: 'text=' + encodeURIComponent(textValue),})
                                .then(response => {
            // Log response details to the console
            console.log('Response Status:', response.status);
            console.log('Response Headers:', response.headers);

            // Return the response for further processing
            return response.text();
        })
        .then(data => {
            // Log the response data to the console
            console.log('Response Data:', data);

            // Access the processed result
            var result = data.result;

        })
                                .catch(error => console.error('Error:', error));
                            }
            
                            function hideForm() {
                                var form = document.getElementById("uploadForm");
                                form.style.display = "none";
                                form = document.getElementById("Textbox");
                                form.style.display = "none";
                                form = document.getElementById("Text1");
                                form.style.display = "none";
                            }
                            function hideForm1(){
                                var form = document.getElementById("<?php echo $mId;?>");
                                form.style.display = "none";
                            }
                        </script>
                        <form id="uploadForm" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post" enctype="multipart/form-data">
                            <label for="file">Select File:</label>
                            <input type="file" name="file" id="file" accept=".stl" style="display: none;" onchange="submitForm()">
                            <button type="button" onclick="document.getElementById('file').click()">Select File</button>
                            <input type="submit" value="Upload" style="display: none;">
                        </form>
                        <h9 id = "Text1"> Or <br>If New Test <br> For Previous 3D Part</h9>
                        <form id ="Textbox" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post">
                            <div class = "Text">
                            <label for="text">Enter Text:</label>
                            </div>
                            <input type="text" name="text" id="text">
                            <div class = "Text">
                            <button type="button" id="submitTextButton" onclick="submitText()">Submit Text</button>
                            </div>
                        </form>
                        <?php
                        $targetDirectory = "STL_Files/"; // Updated directory name
                        $param1 = "";
            
                        if ($_SERVER["REQUEST_METHOD"] == "POST" && isset($_FILES["file"])) {
                            $targetFile = $targetDirectory . basename($_FILES["file"]["name"]);
            
                            if (!file_exists($targetDirectory)) {
                                mkdir($targetDirectory, 0777, true);
                            }
            
                            if (move_uploaded_file($_FILES["file"]["tmp_name"], $targetFile)) {
                                $param1 = "./" . $targetFile;
            
                                // Use escapeshellarg for parameter escaping
                                $escapedParam1 = escapeshellarg($param1);
                                // Construct the command
                                $command = "python test.py $escapedParam1";
            
                                // Execute the command and capture the output
                                $resultAsString = trim(shell_exec($command));
                                $FileName = pathinfo($resultAsString, PATHINFO_FILENAME);
                                $FileName= strtoupper($FileName);
                                $updatedName = mysqli_real_escape_string($conn, $FileName);
                                $Image_Path = substr($resultAsString, 2);

                                $updateQuery = "UPDATE user_image SET Name = '$updatedName', IMG = '$Image_Path', STL = '$targetFile' WHERE IMG = '' AND username = '$username'";
                                
                                $result = mysqli_query($conn, $updateQuery);
                                ?>
                                <img class="image" src="<?php echo $resultAsString; ?>">
                                <script>
                                    hideForm();
                                </script>
                                <div class="Name">
                            <h10 id="<?php echo $HeadId; ?>"><?php echo $FileName ?></h10>
                            <form id="<?php echo $MainId; ?>" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post">
                            <input type="hidden" name="main_id" value="<?php echo $MainId; ?>">
                            </form>

                            <script>
                                document.addEventListener('DOMContentLoaded', function () {
    var form = document.getElementById('<?php echo $MainId; ?>');
    var h10 = document.getElementById('<?php echo $HeadId; ?>');

    console.log(form); // Log the form element to the console

    if (form && h10) {
        h10.addEventListener('click', function () {
            form.submit();
        });
    } else {
        console.error('Form submission error: Element not found');
    }
});
        </script>

                        </div>
                            <?php } else {
                                echo "Sorry, there was an error uploading your file.";
                            }
                        }
                        ?>
                    <?php else : 
                        ?>
                        <img class="image" src="<?= $row['IMG'] ?>">
                        <div>
                            <h10 id="<?php echo $HeadId; ?>"  class = "Name"><?= $row['Name'] ?></h10>
                            
                            <form id="<?php echo $MainId; ?>" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]); ?>" method="post">
                            <input type="hidden" name="main_id" value="<?php echo $MainId; ?>">
                            </form>

                            <script>
                                document.addEventListener('DOMContentLoaded', function () {
    var form = document.getElementById('<?php echo $MainId; ?>');
    var h10 = document.getElementById('<?php echo $HeadId; ?>');

    console.log(form); // Log the form element to the console

    if (form && h10) {
        h10.addEventListener('click', function () {
            form.submit();
        });
    } else {
        console.error('Form submission error: Element not found');
    }
});
        </script>
                        </div>
                    <?php endif; ?>
                </div>
            <?php
            }
            ?> 
    </div>
</body>