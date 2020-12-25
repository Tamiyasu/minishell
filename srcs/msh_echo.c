/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaito <ysaito@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/24 17:35:44 by ysaito            #+#    #+#             */
/*   Updated: 2020/12/25 18:08:05 by ysaito           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
仕様

///////////////////////////////[1]echoのみ
$ echo

$
///////////////////////////////[2]echo 引数
$ echo 42tokyo
42tokyo
$
///////////////////////////////[3]echo 引数（引数間のスペースバラバラ）== (情報を整形)
$ echo 1 2  3   4    5
1 2 3 4 5
$
///////////////////////////////[4]echo -n 引数 == 改行なしで次のプロンプト表示
$ echo -n 42tokyo
42tolyo$
///////////////////////////////[5]シンングルクォーテーション == 囲まれた中は全て文字列とみなす)
$ echo '$PWD'
PWD
$
///////////////////////////////[6]ダブルクォーテーション == 変数の中身を表示する)
$ echo "$PWD"
/Users/user/Desktop/42/42cursus/minishell
$
///////////////////////////////[7]指定された変数が定義されていないとき == 空行のみ表示して改行
$ echo "$PWDPWD"

$
///////////////////////////////[8]シングルクォーテーション一つ or 奇数個でEnter押されたとき == 'が偶数になるまで入力受付 (出力時は'は表示されない)
$ echo 'aaa
> AAA"
> aaa'
aaa
AAA"
aaa
$
///////////////////////////////



*/



int	msh_echo(char **args)
{
	printf("in msh_echo[%s]\n", args[0]);//del

	return (1);
}
