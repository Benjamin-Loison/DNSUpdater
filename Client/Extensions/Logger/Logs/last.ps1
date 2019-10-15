$a = Get-ChildItem "." -Filter *.log
if(($a | Measure-Object | %{$_.Count}) -eq 1)
{
	Get-Content -wait $a
}
Get-Content -wait $a[$a.length - 1]