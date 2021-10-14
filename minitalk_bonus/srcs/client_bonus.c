/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gjacqual <gjacqual@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/08 16:57:46 by gjacqual          #+#    #+#             */
/*   Updated: 2021/10/14 17:33:02 by gjacqual         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minitalk_bonus.h"

static int	check_real_pid(int serv_pid)
{
	if (99 < serv_pid && serv_pid < 99999)
		return (1);
	ft_putstr_fd("Server PID is incorrect or not exist.\n", 1);
	return (0);
}

int	ft_bit_decoder(int symbol, int serv_pid)
{	
	int	count;

	count = 0;
	while (count < 8)
	{
		if ((symbol >> count) & 1)
		{
			if (kill(serv_pid, SIGUSR1) != 0)
			{
				ft_putstr_fd("Error: Serv PID is incorrect!\n", 1);
				return (0);
			}
		}
		else
		{
			if (kill(serv_pid, SIGUSR2) != 0)
			{
				ft_putstr_fd("Error: Serv PID is incorrect!\n", 1);
				return (0);
			}
		}	
		count++;
		usleep(100);
	}
	return (1);
}

int	bit_sender(char *message, int serv_pid)
{
	int	i;

	i = 0;
	while (message[i] != '\0')
	{
		if (ft_bit_decoder(message[i], serv_pid) != 1)
			return (0);
		i++;
	}
	if (ft_bit_decoder(0, serv_pid) != 1)
		return (0);
	return (1);
}

void	ft_server_signal_handler(int sig_nb, siginfo_t *sig_info, void *context)
{
	(void)sig_info;
	(void)context;
	if (sig_nb == SIGUSR1)
	{
		ft_putstr_fd("1 Symbol received!\n", 1);
	}
	if (sig_nb == SIGUSR2)
	{	
		ft_putstr_fd("Signal received!\n", 1);
	}
}

int	main(int argc, char **argv)
{
	struct sigaction	serv_act;
	siginfo_t			sig_info;
	int					serv_pid;

	if (argc != 3)
		ft_putstr_fd("Please enter a correct PID and message after.\n", 1);
	else
	{
		serv_pid = ft_atoi(argv[1]);
		if (check_real_pid(serv_pid))
		{
			serv_act.sa_flags = SA_SIGINFO;
			serv_act.sa_sigaction = &ft_server_signal_handler;
			sigaddset(&serv_act.sa_mask, SIGUSR1);
			sigaddset(&serv_act.sa_mask, SIGUSR2);
			sigaction(SIGUSR1, &serv_act, 0);
			sigaction(SIGUSR2, &serv_act, 0);
			sig_info.si_pid = serv_pid;
			if (bit_sender(argv[2], serv_pid))
				ft_putstr_fd("Message has been sent\n", 1);
			else
				ft_putstr_fd("Error: Message has not been sent\n", 1);
		}
	}
	return (0);
}