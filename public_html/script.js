
function doComputation()
{
    document.getElementById("feur").innerHTML = document.getElementById("myInput").value;

    document.getElementById("strlenValue").innerHTML = document.getElementById("strlenInput").value.length;
    document.getElementById("strcmpValue").innerHTML = document.getElementById("strcmpInput").value.localeCompare(document.getElementById("strcmpInput2").value);
}
